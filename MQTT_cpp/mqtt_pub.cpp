#include <mosquittopp.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

class MyPublisher : public mosqpp::mosquittopp {
public:
    MyPublisher(const char *id) : mosqpp::mosquittopp(id) {}

    void start(const char* topic) {
        int keepalive = 60;
        connect("localhost", 1883, keepalive);
        loop_start(); // Start the network loop in a separate thread

        int count = 0;
        while (true) {
            std::string message = std::to_string(count++);
            publish(nullptr, topic, message.length(), message.c_str());
            std::cout << topic << " " << message.c_str() << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main() {
    mosqpp::lib_init();
    MyPublisher publisher("publisher");
    publisher.start("resource/cpu_usage");

    // Keep the main function from exiting immediately
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    mosqpp::lib_cleanup();
}