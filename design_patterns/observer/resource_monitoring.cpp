#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <nlohmann/json.hpp>

using namespace std;

// Threshold values for resource utilization
int cpu_utilization_threshold = 80;
int memory_utilization_threshold = 80;
int pod_respawn_cpu_threshold = 20;
int pod_respawn_memory_threshold = 20;
int respawn_counter = 0;

// Status flags
inline bool is_pod_running = false;
inline bool is_cpu_utilization_high = false;
inline bool is_memory_utilization_high = false;
inline bool is_cpu_utilization_below_pod_respawn_threshold = false;
inline bool is_memory_utilization_below_pod_respawn_threshold = false;

// Observer interface
class Observer {
public:
    virtual ~Observer() {}
    virtual void update(float avg_cpuUtilization, float avg_memoryUtilization) = 0;
};

// Subject interface
class Subject {
public:
    virtual ~Subject() {}
    virtual void addObserver(Observer* o) = 0;
    virtual void removeObserver(Observer* o) = 0;
    virtual void notifyObservers() = 0;
};

// ResourceMonitorHandler acts as the Subject
class ResourceMonitorHandler : public Subject {
private:
    std::vector<Observer*> observers;
    float avg_cpuUtilization;
    float avg_memoryUtilization;
    bool is_cpu_utilization_high;
    bool is_memory_utilization_high;
    bool is_cpu_utilization_below_pod_respawn_threshold;
    bool is_memory_utilization_below_pod_respawn_threshold;

public:
    void addObserver(Observer* o) override {
        observers.push_back(o);
    }

    void removeObserver(Observer* o) override {
        observers.erase(std::remove(observers.begin(), observers.end(), o), observers.end());
    }

    void notifyObservers() override {
        for (Observer* o : observers) {
            o->update(avg_cpuUtilization, avg_memoryUtilization);
        }
    }

    float getCpuUtilization() {
        const char* command = "python3 ./cpu_utilization.py";
        int result = std::system(command);

        std::ifstream file("cpu_utilization_data.txt");
        std::vector<float> cpu_utilization_values; 

        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                cpu_utilization_values.push_back(std::stof(line)); 
            }
            file.close();
        } else {
            std::cerr << "Unable to open file" << std::endl;
            return 1;
        }
        float average_cpu_usage = std::accumulate(cpu_utilization_values.begin(), cpu_utilization_values.end(),0.0)/ cpu_utilization_values.size();

        return average_cpu_usage;
    }

    float getMemoryUtilization() {
        ifstream file("/proc/meminfo");
        string line;
        long mem_total = 0, mem_available = 0;

        // Read each line of the file
        while (getline(file, line)) {
            if (line.find("MemTotal:") != string::npos) {
                sscanf(line.c_str(), "MemTotal: %ld", &mem_total);
            }
            else if (line.find("MemAvailable:") != string::npos) {
                sscanf(line.c_str(), "MemAvailable: %ld", &mem_available);
            }
        }
        file.close();

        // Calculate memory utilization percentage
        float memoryUtilization = 100.0 * (1.0 - (float)mem_available / mem_total);
        return memoryUtilization;
    }

    void getResourceUtilization(vector<pair<float, float>>& resource_utilization, int time_delay) {
        // Get CPU utilization for complete system
        float cpu_utilization = getCpuUtilization();

        if(cpu_utilization > 100){
            cpu_utilization = 100;
        }
        
        // Get memory utilization
        float memory_utilization = getMemoryUtilization();
        
        // Store CPU and memory utilization
        resource_utilization.push_back({cpu_utilization, memory_utilization});
    }

    void averageResourceUtilization() {
        int time_delay = 1;
        int sample_size = 5;

        vector<float> cpu_utilization;
        vector<float> memory_utilization;
        vector<pair<float, float>> resource_utilization;
        
        while (resource_utilization.size() < sample_size+1) {
            getResourceUtilization(resource_utilization, time_delay);

            for (auto& utilization : resource_utilization) {
                cpu_utilization.push_back(utilization.first);
                memory_utilization.push_back(utilization.second);
            }

            // Calculate average resource utilization over a sample size
            if(sample_size == resource_utilization.size()){
                avg_cpuUtilization = 0;
                avg_memoryUtilization = 0;

                // Average CPU and memory utilization
                avg_cpuUtilization = accumulate(cpu_utilization.begin(), cpu_utilization.end(), 0.0)/cpu_utilization.size();
                avg_memoryUtilization = accumulate(memory_utilization.begin(), memory_utilization.end(), 0.0)/memory_utilization.size();

                cout << "Average CPU Utilization: " << avg_cpuUtilization << "%" << endl;
                cout << "Average Memory Utilization: " << avg_memoryUtilization << "%" << endl;
            }
        }
        notifyObservers();
    }
};

// DeletePod acts as the Observer
class DeletePod : public Observer {
public:
    void update(float avg_cpuUtilization, float avg_memoryUtilization) override {
        // Logic to check if the resource utilization is high
        if(avg_cpuUtilization >= cpu_utilization_threshold){
            cout << "CPU utilization is high" << endl;
            is_cpu_utilization_high = true;
        } else{
            cout<< "NONE" << endl;
            is_cpu_utilization_high = false;
        }

        if(avg_memoryUtilization >= memory_utilization_threshold){
            cout << "Memory utilization is high" << endl;
            is_memory_utilization_high = true;
        } else{
            cout<< "NONE" << endl;
            is_memory_utilization_high = false;
        }

        // Check if CPU and Memory utilization is below threshold
        if(is_cpu_utilization_high or is_memory_utilization_high){
            string msg = "Resource utilization is above threshold limit";
            deletePod();    // Delete QM pod

            // if (!command.empty()) {
            //     system(command.c_str());
            // }

            // LOG(ERROR) << msg;
            cout << msg << endl;
        }
    }

    void deletePod() {
        cout<< "Pod has been successfully deleted" << endl;
        // Logic to delete a pod
    }
};

// Spawn acts as the Observer
class RespawnPod : public Observer {
public:
    void update(float avg_cpuUtilization, float avg_memoryUtilization) override {
        // Logic to check if the resource utilization is high
        if(avg_cpuUtilization < pod_respawn_cpu_threshold){
            cout << "CPU utilization is below threshold" << endl;
            is_cpu_utilization_below_pod_respawn_threshold = true;
        } else{
            cout<< "NONE" << endl;
            is_cpu_utilization_below_pod_respawn_threshold = false;
        }

        if(avg_memoryUtilization < pod_respawn_memory_threshold){
            cout << "Memory utilization is below threshold" << endl;
            is_memory_utilization_below_pod_respawn_threshold = true;
        } else{
            cout<< "NONE" << endl;
            is_memory_utilization_below_pod_respawn_threshold = false;
        }

        // Check if CPU and Memory utilization is below threshold
        if(!(is_cpu_utilization_high or is_memory_utilization_high)){
            // Check if pod is already running
            if(is_memory_utilization_below_pod_respawn_threshold and is_cpu_utilization_below_pod_respawn_threshold) {
                respawn_counter++;
                if(respawn_counter >= 10) {
                    respawnPod();    // Apply QM pod
                    string msg = "Spawnning QM pod as CPU and Memory utilization is below threshold";
                    cout << msg << endl;
                    respawn_counter = 0;
                }
            } else {
                respawn_counter = 0;
            }
        }
    }

    void respawnPod() {
        cout<< "Pod has been successfully respawned" << endl;
        // Logic to spawn a new pod
    }
};

int main() {
    ResourceMonitorHandler resourceMonitor;
    DeletePod deletePod;
    RespawnPod respawnPod;

    // Add Deletepod, RespawnPod as an observer to ResourceMonitorHandler
    resourceMonitor.addObserver(&deletePod);
    resourceMonitor.addObserver(&respawnPod);

    // while(true){
        // Start monitoring resources
    resourceMonitor.averageResourceUtilization();
    // }    

    return 0;
}