import subprocess

def add_clock_skew(skew_seconds):
    try:
        # Run the command to adjust the system time
        subprocess.run(['sudo', 'date', '-s', f'+{skew_seconds} seconds'])
        print("Clock skew added successfully.")
    except Exception as e:
        print("Error:", e)

# Example usage: Add 10 seconds of clock skew
add_clock_skew(10)