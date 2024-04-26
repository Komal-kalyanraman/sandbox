use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn main() -> io::Result<()> {
    let path = Path::new("/proc/meminfo");
    let file = File::open(&path)?;
    let reader = io::BufReader::new(file);

    let mut mem_total: i64 = 0;
    let mut mem_available: i64 = 0;

    for line in reader.lines() {
        let line = line?;
        if line.starts_with("MemTotal:") {
            let parts: Vec<&str> = line.split_whitespace().collect();
            mem_total = parts[1].parse::<i64>().unwrap(); // Convert from KB to bytes
        } else if line.starts_with("MemAvailable:") {
            let parts: Vec<&str> = line.split_whitespace().collect();
            mem_available = parts[1].parse::<i64>().unwrap(); // Convert from KB to bytes
        }
    }

    let mem_used = format!("{:.2}", 100.0 * (1.0 - (mem_available as f64 / mem_total as f64)));
    
    println!("MemUsed: {} bytes", mem_used);
    println!("MemTotal: {} bytes", mem_total);
    println!("MemAvailable: {} bytes", mem_available);

    Ok(())
}