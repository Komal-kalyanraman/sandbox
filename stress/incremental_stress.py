import subprocess
import time
import os

# Function to get total memory in KB
def get_total_memory_kb():
    with open('/proc/meminfo', 'r') as meminfo:
        for line in meminfo:
            if 'MemTotal' in line:
                total_memory_kb = int(line.split()[1])
                return total_memory_kb
    return 0

# Calculate memory stress in bytes for a given percentage of total memory
def calculate_memory_stress_bytes(percentage):
    total_memory_kb = get_total_memory_kb()
    # Calculate the memory to stress in KB
    memory_stress_kb = total_memory_kb * (percentage / 100)
    # Convert KB to bytes
    return int(memory_stress_kb) * 1024

# Duration for each stress test
duration = "2s"

# Loop from 1% to 20% incrementally for both CPU and memory
for percentage in range(1, 31):
    # Calculate memory stress in bytes for the current percentage
    memory_stress_bytes = int(calculate_memory_stress_bytes(percentage)/(1024*1024))
    memory_stress_arg = str(memory_stress_bytes) + "M"  # Stress-ng expects a string like "3200M" or "1024K"
    print(memory_stress_arg)

    # Construct the stress-ng command
    command = [
        "stress-ng",
        "--cpu", "0",
        "--cpu-load", str(percentage),
        "--vm", "1",
        "--vm-bytes", memory_stress_arg,
        "--timeout", duration
    ]

    # command = [
    #     "stress-ng",
    #     "--cpu", "0",
    #     "--cpu-load", "1",
    #     "--vm", "1",
    #     "--vm-bytes", "160M",
    #     "--timeout", duration
    # ]
    
    # Execute the command
    print(f"Executing command: {' '.join(command)}")
    subprocess.run(command)

command = "pkill", "stress-ng"
subprocess.run(command)