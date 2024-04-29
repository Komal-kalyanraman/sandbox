#include <mosquittopp.h>
#include <iostream>
#include <chrono>
#include <thread>

class MySubscriber : public mosqpp::mosquittopp {
public:
    MySubscriber(const char *id, const char *topic) : mosqpp::mosquittopp(id) {
        int keepalive = 60;
        connect("localhost", 1883, keepalive);
        subscribe(nullptr, topic);
        loop_start(); // Start the network loop
    }

    void on_message(const struct mosquitto_message *message) {
        std::cout << message->topic << " " << static_cast<char*>(message->payload) << "\n";
    }
};

int main() {
    mosqpp::lib_init();
    MySubscriber subscriber("subscriber", "test/topic");

    // Keep the main function from exiting immediately
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    mosqpp::lib_cleanup();
}