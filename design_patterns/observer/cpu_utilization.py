import psutil
import time

def write_cpu_utilization_to_file():
    with open("cpu_utilization_data.txt", "w") as file:
        cpu_utilization = psutil.cpu_percent(interval=0.02, percpu=True)
        # print("Per Core CPU Utilization:")
        for core, utilization in enumerate(cpu_utilization):
            # print(f"Core {core}: {utilization}%")
            file.write(str(utilization) + "\n")
        # print("\n")

if __name__ == "__main__":
    write_cpu_utilization_to_file()