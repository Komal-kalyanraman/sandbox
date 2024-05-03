pub mod resource_utilization {
    tonic::include_proto!("resource_utilization");
}

use tonic::{transport::Server, Request, Response, Status};
use resource_utilization::resource_utilization_server::{ResourceUtilization, ResourceUtilizationServer};
use resource_utilization::{ResourceUtilizationRequest, ResourceUtilizationResponse};
use futures::stream::Stream;
use std::pin::Pin;
use futures::stream::repeat_with;
use rand::Rng;
use rand::rngs::StdRng;
use rand::SeedableRng;

// defining a struct for our service
#[derive(Default)]
pub struct MyResourceUtilization {}

// implementing rpc for service defined in .proto
#[tonic::async_trait]
impl ResourceUtilization for MyResourceUtilization {
    type SendStream = Pin<Box<dyn Stream<Item = Result<ResourceUtilizationResponse, Status>> + Send + Sync + 'static>>;

    async fn send(&self, request: Request<ResourceUtilizationRequest>) -> Result<Response<Self::SendStream>, Status> {
        let mut rng = StdRng::from_entropy();
        let output_stream = repeat_with(move || {
            let random_number: u32 = rng.gen();
            Ok(ResourceUtilizationResponse { cpu_resource: random_number.clone() })
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