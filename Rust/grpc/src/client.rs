pub mod resource_utilization {
    tonic::include_proto!("resource_utilization");
}

use resource_utilization::resource_utilization_client::ResourceUtilizationClient;
use resource_utilization::ResourceUtilizationRequest;
use tonic::Streaming;
use resource_utilization::ResourceUtilizationResponse;
use rumqttc::{MqttOptions, Client, QoS};
use tokio::runtime::Runtime;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // creating a channel ie connection to server
    let channel = tonic::transport::Channel::from_static("http://[::1]:50051")
        .connect()
        .await?;
    // creating gRPC client from channel
    let mut grpc_client = ResourceUtilizationClient::new(channel);
    // creating a new Request
    let request = tonic::Request::new(
        ResourceUtilizationRequest {
        },
    );
    // sending request and waiting for response
    let mut response_stream: Streaming<ResourceUtilizationResponse> = grpc_client.send(request).await?.into_inner();

    // MQTT setup
    let mut mqttoptions = MqttOptions::new("rust_client", "localhost", 1883); // Change this to your MQTT broker address and desired client ID
    mqttoptions.set_keep_alive(5);

    let (mut mqtt_client, mut connection) = Client::new(mqttoptions, 10);

    let rt = Runtime::new().unwrap();
    rt.spawn(async move {
        for notification in connection.iter() {
            println!("{:?}", notification);
        }
    });
    
    // Loop to continuously print incoming messages
    while let Some(response) = response_stream.message().await? {
        println!("CPU Usage: {}", response.cpu_resource);
        println!("Memory Usage: {}", response.memory_resource);
        println!("Timestamp: {}", response.timestamp);

        let topic = "cpu_usage";
        let qos = QoS::AtLeastOnce;
        let retain = false;
        let payload = response.cpu_resource.to_string();

        // mqtt_client.publish(topic, qos, retain, payload).unwrap();
    }

    Ok(())
}