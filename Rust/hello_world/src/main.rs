use std::thread;
use std::time::Duration;
use std::sync::{Arc, Mutex};
use tokio::runtime::Runtime;
use sysinfo::{ProcessorExt, System, SystemExt};
use rumqttc::{MqttOptions, Client, QoS};

fn cpu_utilization_and_memory(client: Arc<Mutex<Client>>) {
    let mut system = System::new_all();
    let mut cpu_utilization_values = Vec::new();

    // We sleep 20ms to get the CPU usage.
    thread::sleep(Duration::from_millis(20));
    system.refresh_all();

    for (_core, processor) in system.processors().iter().enumerate() {
        cpu_utilization_values.push(processor.cpu_usage());
    }
    let sum: f32 = cpu_utilization_values.iter().sum();
    let average_cpu_usage = (sum / cpu_utilization_values.len() as f32) as i32;
    let average_cpu_usage_string = average_cpu_usage.to_string();
    let average_cpu_usage_bytes = average_cpu_usage_string.into_bytes();

    // Get total and free memory
    let total_memory = system.total_memory();
    let free_memory = system.available_memory();
    let memory_usage = format!("{:.2}", ((total_memory as f32 - free_memory as f32)/total_memory as f32)* 100.0);

    println!("Average CPU usage: {}%", average_cpu_usage);
    println!("Memory usage: {}%", memory_usage);

    // Publish the CPU and memory usage data
    let rt = Runtime::new().unwrap();
    rt.block_on(async {
        let client = Arc::clone(&client);
        tokio::spawn(async move {
            let mut client = client.lock().unwrap();
            client.publish("resource/cpu_usage", QoS::AtLeastOnce, false, average_cpu_usage_bytes).unwrap();
        }).await.unwrap();
    });
}

fn main() {
    // Create a new MQTT client
    let mut mqttoptions = MqttOptions::new("publisher", "localhost", 1883);
    mqttoptions.set_keep_alive(60);

    let (client, mut connection) = Client::new(mqttoptions, 10);
    let client = Arc::new(Mutex::new(client));

    // Start a new thread to handle network events
    thread::spawn(move || {
        for event in connection.iter() {
            match event {
                Ok(v) => println!("Event: {:?}", v),
                Err(e) => println!("Error: {:?}", e),
            }
        }
    });

    loop {
        cpu_utilization_and_memory(Arc::clone(&client));
    }
}