#include <iostream>
#include "socket_producer.h"

int main() 
{
    socket_producer producer(8080);
    
    if (!producer.initialize())
        return 1;
    
    std::cout << "Producer server running. Press Ctrl+C to stop.\n";
    producer.run(10); // Produce 10 students
    
    return 0;
}