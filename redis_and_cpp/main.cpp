#include <iostream>
#include <hiredis/hiredis.h>

int main() {
    // Connect to Redis
    redisContext* c = redisConnect("localhost", 6379);
    if (c == NULL || c->err) {
        if (c) {
            std::cout << "Error: " << c->errstr << std::endl;
        } else {
            std::cout << "Can't allocate redis context" << std::endl;
        }
        return 1;
    }

    // Write data to Redis
    redisReply* reply = (redisReply*)redisCommand(c, "SET %s %s", "name", "komal");
    freeReplyObject(reply);

    // Read data from Redis
    reply = (redisReply*)redisCommand(c, "GET %s", "name");
    if (reply->type == REDIS_REPLY_STRING) {
        std::cout << "Got value: " << reply->str << std::endl;
    }
    freeReplyObject(reply);

    // Disconnect from Redis
    redisFree(c);

    return 0;
}