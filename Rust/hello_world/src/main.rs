use std::fs::File;
use std::io::Write;
use std::io::{self, BufRead};
use std::path::Path;
use std::thread;
use std::time::Duration;
use sysinfo::{ProcessorExt, System, SystemExt};

fn write_cpu_utilization_to_file() {
    let mut system = System::new_all();
    // We sleep 20ms to get the CPU usage.
    thread::sleep(Duration::from_millis(20));
    system.refresh_all();

    let mut file = File::create("cpu_utilization_data.txt").expect("Unable to create file");
    for (core, processor) in system.processors().iter().enumerate() {
        writeln!(file, "Core {}: {}%", core, processor.cpu_usage())
            .expect("Unable to write data");
    }
}

fn read_cpu_utilization_from_file() -> io::Result<()> {
    let path = Path::new("cpu_utilization_data.txt");
    let file = File::open(&path)?;
    let reader = io::BufReader::new(file);

    let mut cpu_utilization_values = Vec::new();

    for line in reader.lines() {
        let line = line?;
        println!("Line: {}", line);
        let parts: Vec<&str> = line.split(':').collect();
        if parts.len() == 2 {
            let value_str = parts[1].trim_end_matches('%').trim();
            let value: f32 = value_str.parse().unwrap_or_default();
            println!("CPU utilization value: {}", value);
            cpu_utilization_values.push(value);
        }
    }

    let sum: f32 = cpu_utilization_values.iter().sum();
    let average_cpu_usage = sum / cpu_utilization_values.len() as f32;

    println!("Average CPU usage: {}%", average_cpu_usage);

    Ok(())
}

fn main() {
    write_cpu_utilization_to_file();
    if let Err(err) = read_cpu_utilization_from_file() {
        eprintln!("Error: {}", err);
    }
}