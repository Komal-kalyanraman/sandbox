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
use futures::stream::repeat_with;

#[derive(Default)]
pub struct MyResourceUtilization {
    cpu_usage: Arc<Mutex<u32>>,
}

impl MyResourceUtilization {
    pub fn new() -> Self {
        let cpu_usage = Arc::new(Mutex::new(0));
        let cpu_usage_clone = Arc::clone(&cpu_usage);

        thread::spawn(move || {
            let mut system = System::new_all();
            loop {
                system.refresh_cpu();
                thread::sleep(Duration::from_millis(50));

                let mut total_cpu_usage = 0.0;
                for processor in system.processors() {
                    total_cpu_usage += processor.cpu_usage();
                }
                total_cpu_usage /= system.processors().len() as f32;

                let mut cpu_usage_lock = cpu_usage_clone.lock().unwrap();
                *cpu_usage_lock = total_cpu_usage as u32;
            }
        });

        Self { cpu_usage }
    }
}

#[tonic::async_trait]
impl ResourceUtilization for MyResourceUtilization {
    type SendStream = Pin<Box<dyn Stream<Item = Result<ResourceUtilizationResponse, Status>> + Send + Sync + 'static>>;
    
    async fn send(&self, _request: Request<ResourceUtilizationRequest>) -> Result<Response<Self::SendStream>, Status> {
        let cpu_usage_clone = Arc::clone(&self.cpu_usage);

        let output_stream = repeat_with(move || {
            let cpu_usage_lock = cpu_usage_clone.lock().unwrap();
            let response = ResourceUtilizationResponse { cpu_resource: *cpu_usage_lock };
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