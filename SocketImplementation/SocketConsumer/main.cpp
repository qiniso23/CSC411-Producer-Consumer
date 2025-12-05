#include <thread>
#include <iostream>
#include "consumer/socket_consumer.h"

int main() 
{
    socket_consumer consumer("127.0.0.1", 8080);
    
    if (!consumer.initialize())
        return 1;
    
    std::cout << "Consumer client starting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // wait for server
    consumer.run(10); // consume 10 students
    
    return 0;
}