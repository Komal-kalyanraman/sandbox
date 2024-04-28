#include <mosquittopp.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

class MyMosq : public mosqpp::mosquittopp
{
public:
    MyMosq(const char *id, const char *host, int port);
    ~MyMosq();

    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void publish_message(const char* _message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
};

MyMosq::MyMosq(const char *id, const char *host, int port) : mosquittopp(id, true)
{
    int keepalive = 60;
    connect_async(host, port, keepalive);
};

MyMosq::~MyMosq()
{
}

void MyMosq::on_connect(int rc)
{
    if (rc == 0)
    {
        std::cout << "Connected with code " << rc << ".\n";
        subscribe(NULL, "test/topic");

        // Publish a message to the "test/topic" topic
        std::string message = "Hello, MQTT!";
        publish(NULL, "test/topic", message.size(), message.c_str());
    }
    else
    {
        std::cout << "Connect failed with code " << rc << ".\n";
    }
}

void MyMosq::on_message(const struct mosquitto_message *message)
{
    std::string msg(static_cast<char*>(message->payload), message->payloadlen);
    std::cout << "Received message: " << msg << "\n";
}

void MyMosq::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    std::cout << "Subscription succeeded.\n";
}

int main(int argc, char *argv[])
{
    mosqpp::lib_init();

    MyMosq *mosq = new MyMosq("test", "localhost", 1883);

    // Publish a message to the "test/topic" topic
    std::string message = "Hello, MQTT!";
    mosq->publish(NULL, "test/topic", message.size(), message.c_str());

    int rc = mosq->loop_start();

    if (rc != 0)
    {
        std::cout << "Error: " << mosqpp::strerror(rc) << "\n";
        return rc;
    }

    // Wait for 5 seconds to give the client time to process messages
    sleep(5);

    mosqpp::lib_cleanup();

    return 0;
}