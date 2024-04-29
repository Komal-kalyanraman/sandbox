use std::fs::File;
use std::io::Write;
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

fn main() {
    write_cpu_utilization_to_file();
}