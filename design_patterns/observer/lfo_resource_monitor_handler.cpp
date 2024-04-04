
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
// #include <glog/logging.h>

#include "inc/lfo_common.hpp"
#include "inc/lfo_redis_handler.hpp"
#include "inc/lfo_server_handler.hpp"
#include "inc/lfo_resource_monitor_handler.hpp"

using namespace std;

ResourceMonitorHandler& ResourceMonitorHandler::GetInstance()
{
    static ResourceMonitorHandler self{};
    return self;
}

// Function to get CPU utilization of enire system
float GetCpuUtilization(){
    const char* command = "python3 ../bl/src/cpu_utilization.py";
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
    float average_cpu_usage= std::accumulate(cpu_utilization_values.begin(), cpu_utilization_values.end(),0.0)/ cpu_utilization_values.size();

    return average_cpu_usage;
}

string ReadStressRequest(){
    string stress_request = RedisDBHandler::GetInstance().ReadData("stress_request");
    return stress_request;
}

// Function to get memory utilization
float GetMemoryUtilization() {
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
    float mem_utilization = 100.0 * (1.0 - (float)mem_available / mem_total);
    return mem_utilization;
}

void ResourceMonitorHandler::GetResourceUtilization(vector<pair<float, float>>& resource_utilization, int time_delay){
    // Get CPU utilization for complete system
    float cpu_utilization = GetCpuUtilization();

    if(cpu_utilization > 100){
        cpu_utilization = 100;
    }
    
    // Get memory utilization
    float memory_utilization = GetMemoryUtilization();
    
    // Store CPU and memory utilization
    resource_utilization.push_back({cpu_utilization, memory_utilization});
    
    // Sleep for a short duration before polling again
    // this_thread::sleep_for(chrono::seconds(time_delay));
}

void ResourceMonitorHandler::AverageResourceUtilization(){
    
    // Get resource monitor time interval and sample size from config file
    int time_delay = stoi(config_parameters["resource_monitor_time_interval"]);
    int sample_size = stoi(config_parameters["resource_monitor_sample_size"]);
    
    int cpu_utilization_threshold = stoi(config_parameters["cpu_utilization_threshold"]);
    int memory_utilization_threshold = stoi(config_parameters["memory_utilization_threshold"]);

    int pod_respawn_cpu_threshold = stoi(config_parameters["pod_respawn_cpu_threshold"]);
    int pod_respawn_memory_threshold = stoi(config_parameters["pod_respawn_memory_threshold"]);
    
    vector<float> cpu_utilization;
    vector<float> memory_utilization;
    vector<pair<float, float>> resource_utilization;

    while(resource_utilization.size() < sample_size+1){
        
        GetResourceUtilization(resource_utilization, time_delay);

        for (auto& utilization : resource_utilization) {
            cpu_utilization.push_back(utilization.first);
            memory_utilization.push_back(utilization.second);
        }

        // Calculate average resource utilization over a sample size
        if(sample_size == resource_utilization.size()){
            int avg_cpu = 0;
            int avg_memory = 0;

            // Average CPU and memory utilization
            avg_cpu = accumulate(cpu_utilization.begin(), cpu_utilization.end(), 0.0)/cpu_utilization.size();
            avg_memory = accumulate(memory_utilization.begin(), memory_utilization.end(), 0.0)/memory_utilization.size();

            cout << "Average CPU Utilization: " << avg_cpu << "%" << endl;
            cout << "Average Memory Utilization: " << avg_memory << "%" << endl;

            if(avg_cpu >= cpu_utilization_threshold){
                cout << "CPU utilization is high" << endl;
                is_cpu_utilization_high = true;
            } else{
                is_cpu_utilization_high = false;
            }

            if(avg_cpu < pod_respawn_cpu_threshold){
                cout << "CPU utilization is below threshold" << endl;
                is_cpu_utilization_below_pod_respawn_threshold = true;
            } else{
                is_cpu_utilization_below_pod_respawn_threshold = false;
            }

            if(avg_memory >= memory_utilization_threshold){
                cout << "Memory utilization is high" << endl;
                is_memory_utilization_high = true;
            } else{
                is_memory_utilization_high = false;
            }

            if(avg_memory < pod_respawn_memory_threshold){
                cout << "Memory utilization is below threshold" << endl;
                is_memory_utilization_below_pod_respawn_threshold = true;
            } else{
                is_memory_utilization_below_pod_respawn_threshold = false;
            }
        }
    }
}

void ResourceMonitorHandler::Init(){

    ServerHandler::GetInstance().ApplyPod();    // Spawnning QM pod
    int respawn_counter = 0;
    
    while (true) {
        AverageResourceUtilization();   // Get average CPU, Memory resource utilization
        
        string command = RedisDBHandler::GetInstance().ReadData("stress_request");  // Read stress_request from Redis
        
        cout<<"Pod running status: "<<is_pod_running<<endl;
        cout<<"CPU high utilization: "<<is_cpu_utilization_high<<endl;
        cout<<"Memory high utilization: "<<is_memory_utilization_high<<endl;

        // Check if CPU and Memory utilization is below threshold
        if(!(is_cpu_utilization_high or is_memory_utilization_high)){
            // Check if pod is already running
            if(!(ServerHandler::GetInstance().CheckPodName(qm_1_pod_name) == true)) {
                if(is_memory_utilization_below_pod_respawn_threshold and is_cpu_utilization_below_pod_respawn_threshold) {
                    respawn_counter++;
                    if(respawn_counter >= 10) {
                        ServerHandler::GetInstance().ApplyPod();    // Apply QM pod
                        string msg = "Spawnning QM pod as CPU and Memory utilization is below threshold";
                        // LOG(INFO) << msg;
                        cout << msg << endl;
                        respawn_counter = 0;
                    }
                } else {
                    respawn_counter = 0;
                }
            }
        }
        else{
            string msg = "Resource utilization is above threshold limit";
            ServerHandler::GetInstance().DeletePod();    // Delete QM pod

            if (!command.empty()) {
                system(command.c_str());
            }

            // LOG(ERROR) << msg;
            cout << msg << endl;
        }
    }
}
