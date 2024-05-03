pub mod resource_utilization {
    tonic::include_proto!("resource_utilization");
}

use resource_utilization::say_client::SayClient;
use resource_utilization::ResourceUtiliazationRequest;
use tonic::Streaming;
use resource_utilization::ResourceUtiliazationResponse;
use tonic::Status;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // creating a channel ie connection to server
    let channel = tonic::transport::Channel::from_static("http://[::1]:50051")
        .connect()
        .await?;
    // creating gRPC client from channel
    let mut client = SayClient::new(channel);
    // creating a new Request
    let request = tonic::Request::new(
        ResourceUtiliazationRequest {
        },
    );
    // sending request and waiting for response
    let mut response_stream: Streaming<ResourceUtiliazationResponse> = client.send(request).await?.into_inner();

    // Loop to continuously print incoming messages
    while let Some(response) = response_stream.message().await? {
        println!("RESPONSE={:?}", response);
    }

    Ok(())
}