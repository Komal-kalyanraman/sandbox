pub mod resource_utilization {
    tonic::include_proto!("resource_utilization");
}

use std::sync::{Arc, Mutex};
use std::thread;
use std::time::Duration;
use tonic::{transport::Server, Request, Response, Status};
use resource_utilization::resource_utilization_server::{ResourceUtilization, ResourceUtilizationServer};
use resource_utilization::{ResourceUtilizationRequest, ResourceUtilizationResponse};
use futures::stream::Stream;
use std::pin::Pin;
use sysinfo::{ProcessorExt, System, SystemExt};
use futures::stream::StreamExt;
use std::time::SystemTime;
use chrono::{DateTime, Local};

#[derive(Default)]
pub struct MyResourceUtilization {
    cpu_usage: Arc<Mutex<u32>>,
    memory_usage: Arc<Mutex<u32>>,
    timestamp: Arc<Mutex<String>>,
}

impl MyResourceUtilization {
    pub fn new() -> Self {
        let cpu_usage = Arc::new(Mutex::new(0));
        let memory_usage = Arc::new(Mutex::new(0));
        let timestamp = Arc::new(Mutex::new(String::new()));

        let cpu_usage_clone = Arc::clone(&cpu_usage);
        let memory_usage_clone = Arc::clone(&memory_usage);
        let timestamp_clone = Arc::clone(&timestamp);

        thread::spawn(move || {
            let mut system = System::new_all();
            loop {
                system.refresh_cpu();
                system.refresh_memory();

                let mut total_cpu_usage = 0.0;
                for processor in system.processors() {
                    total_cpu_usage += processor.cpu_usage();
                }
                total_cpu_usage /= system.processors().len() as f32;
                
                // Get total and free memory
                let total_memory = system.total_memory();
                let free_memory = system.available_memory();
                let memory_usage: u32 = (((total_memory as f32 - free_memory as f32)/total_memory as f32)* 100.0) as u32;

                // Get the current system time
                let system_time = SystemTime::now();
                let datetime: DateTime<Local> = DateTime::from(system_time);
                let system_time_string = datetime.format("%Y-%m-%d %H:%M:%S%.3f").to_string();
                
                thread::sleep(Duration::from_millis(50));

                let mut cpu_usage_lock = cpu_usage_clone.lock().unwrap();
                *cpu_usage_lock = total_cpu_usage as u32;

                let mut memory_usage_lock = memory_usage_clone.lock().unwrap();
                *memory_usage_lock = memory_usage;

                let mut timestamp_lock = timestamp_clone.lock().unwrap();
                *timestamp_lock = system_time_string;
            }
        });

        Self { cpu_usage, memory_usage, timestamp }
    }
}

#[tonic::async_trait]
impl ResourceUtilization for MyResourceUtilization {
    type SendStream = Pin<Box<dyn Stream<Item = Result<ResourceUtilizationResponse, Status>> + Send + Sync + 'static>>;
    
    async fn send(&self, _request: Request<ResourceUtilizationRequest>) -> Result<Response<Self::SendStream>, Status> {
        let cpu_usage_clone = Arc::clone(&self.cpu_usage);
        let memory_usage_clone = Arc::clone(&self.memory_usage);
        let timestamp_clone = Arc::clone(&self.timestamp);

        let output_stream = tokio::time::interval(Duration::from_millis(50))
            .map(move |_| {
                let cpu_usage_lock = cpu_usage_clone.lock().unwrap();
                let memory_usage_lock = memory_usage_clone.lock().unwrap();
                let timestamp_lock = timestamp_clone.lock().unwrap();

                let response = ResourceUtilizationResponse { 
                    cpu_resource: *cpu_usage_lock, 
                    memory_resource: *memory_usage_lock, 
                    timestamp: timestamp_lock.clone() 
                };
                Ok(response)
            });

        Ok(Response::new(Box::pin(output_stream)))
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // defining address for our service
    let addr = "[::1]:50051".parse().unwrap();
    // creating a service
    let resource_utilization = MyResourceUtilization::new();
    println!("Server listening on {}", addr);
    // adding our service to our server.
    Server::builder()
        .add_service(ResourceUtilizationServer::new(resource_utilization))
        .serve(addr)
        .await?;
    Ok(())
}