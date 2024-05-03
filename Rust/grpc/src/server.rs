pub mod resource_utilization {
    tonic::include_proto!("resource_utilization");
}

use std::time::Duration;
use std::thread;
use tonic::{transport::Server, Request, Response, Status};
use resource_utilization::resource_utilization_server::{ResourceUtilization, ResourceUtilizationServer};
use resource_utilization::{ResourceUtilizationRequest, ResourceUtilizationResponse};
use futures::stream::Stream;
use std::pin::Pin;
use sysinfo::{ProcessorExt, System, SystemExt};
use futures::stream::repeat_with;

// defining a struct for our service
#[derive(Default)]
pub struct MyResourceUtilization {}

// implementing rpc for service defined in .proto
#[tonic::async_trait]
impl ResourceUtilization for MyResourceUtilization {
    type SendStream = Pin<Box<dyn Stream<Item = Result<ResourceUtilizationResponse, Status>> + Send + Sync + 'static>>;
    
    async fn send(&self, request: Request<ResourceUtilizationRequest>) -> Result<Response<Self::SendStream>, Status> {
        let mut system = System::new_all();
        let mut resource_utilization_values = Vec::new();

        // let mut rng = StdRng::from_entropy();
        let output_stream = repeat_with(move || {
            resource_utilization_values.clear();
            system.refresh_cpu();
            thread::sleep(Duration::from_secs(1));

            for (_core, processor) in system.processors().iter().enumerate() {
                resource_utilization_values.push(processor.cpu_usage());
                print!("{} ", processor.cpu_usage());
            }
            
            let sum: f32 = resource_utilization_values.iter().sum();
            let total_cpu_usage = (sum / resource_utilization_values.len() as f32) as u32;
            Ok(ResourceUtilizationResponse { cpu_resource: total_cpu_usage.clone() })
        });

        Ok(Response::new(Box::pin(output_stream)))
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // defining address for our service
    let addr = "[::1]:50051".parse().unwrap();
    // creating a service
    let resource_utilization: MyResourceUtilization = MyResourceUtilization::default();
    println!("Server listening on {}", addr);
    // adding our service to our server.
    Server::builder()
        .add_service(ResourceUtilizationServer::new(resource_utilization))
        .serve(addr)
        .await?;
    Ok(())
}