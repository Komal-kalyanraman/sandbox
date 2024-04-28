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
    std::cout << "Connected with code " << rc << ".\n";
    if (rc == 0)
    {
        subscribe(NULL, "test/topic", 0);
    }
}

void MyMosq::on_message(const struct mosquitto_message *message)
{
    std::cout << "Got message: " << static_cast<char*>(message->payload) << "\n";
}

void MyMosq::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    std::cout << "Subscription succeeded.\n";
}

int main(int argc, char *argv[])
{
    mosqpp::lib_init();

    MyMosq *mosq = new MyMosq("test", "localhost", 1883);

    int rc = mosq->loop_start();

    if (rc != 0)
    {
        std::cout << "Error: " << mosqpp::strerror(rc) << "\n";
        return rc;
    }

    mosqpp::lib_cleanup();

    return 0;
}