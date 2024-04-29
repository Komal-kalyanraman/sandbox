use std::thread;
use std::time::Duration;
use sysinfo::{ProcessorExt, System, SystemExt};

fn cpu_utilization() {
    let mut system = System::new_all();
    let mut cpu_utilization_values = Vec::new();

    // We sleep 20ms to get the CPU usage.
    thread::sleep(Duration::from_millis(20));
    system.refresh_all();
    
    for (_core, processor) in system.processors().iter().enumerate() {
        cpu_utilization_values.push(processor.cpu_usage());
    }
    let sum: f32 = cpu_utilization_values.iter().sum();
    let average_cpu_usage = sum / cpu_utilization_values.len() as f32;

    println!("Average CPU usage: {}%", average_cpu_usage);
}

fn main() {
    cpu_utilization();
}