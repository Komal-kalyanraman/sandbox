use std::thread;
use std::time::Duration;
use sysinfo::{ProcessorExt, System, SystemExt};

fn cpu_utilization_and_memory() {
    let mut system = System::new_all();
    let mut cpu_utilization_values = Vec::new();

    // We sleep 20ms to get the CPU usage.
    thread::sleep(Duration::from_millis(20));
    system.refresh_all();

    for (_core, processor) in system.processors().iter().enumerate() {
        cpu_utilization_values.push(processor.cpu_usage());
    }
    let sum: f32 = cpu_utilization_values.iter().sum();
    let average_cpu_usage = format!("{:.2}", sum / cpu_utilization_values.len() as f32);

    // Get total and free memory
    let total_memory = system.total_memory();
    let free_memory = system.available_memory();
    let memory_usage = format!("{:.2}", ((total_memory as f32 - free_memory as f32)/total_memory as f32)* 100.0);

    println!("Average CPU usage: {}%", average_cpu_usage);
    println!("Memory usage: {}%", memory_usage);
}

fn main() {
    loop {
        cpu_utilization_and_memory();
    }
}