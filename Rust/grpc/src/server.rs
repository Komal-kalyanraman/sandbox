pub mod hello {
    tonic::include_proto!("hello");
}

use tonic::{transport::Server, Request, Response, Status};
use crate::hello::say_server::{Say, SayServer};
use crate::hello::{SayRequest, SayResponse};
use futures::stream::Stream;
use std::pin::Pin;
use tokio::stream::StreamExt;
use futures::stream::repeat;

// defining a struct for our service
#[derive(Default)]
pub struct MySay {}

// implementing rpc for service defined in .proto
#[tonic::async_trait]
impl Say for MySay {
    type SendStream = Pin<Box<dyn Stream<Item = Result<SayResponse, Status>> + Send + Sync + 'static>>;

    async fn send(&self, request: Request<SayRequest>) -> Result<Response<Self::SendStream>, Status> {
        let message = format!("hello {}", request.get_ref().name);

        let output_stream = repeat(Ok(SayResponse { message: message.clone() }));

        Ok(Response::new(Box::pin(output_stream)))
    }
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // defining address for our service
    let addr = "[::1]:50051".parse().unwrap();
    // creating a service
    let say = MySay::default();
    println!("Server listening on {}", addr);
    // adding our service to our server.
    Server::builder()
        .add_service(SayServer::new(say))
        .serve(addr)
        .await?;
    Ok(())
}