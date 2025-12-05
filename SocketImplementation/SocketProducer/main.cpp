#include <iostream>
#include <filesystem>
#include "network_server.h"
#include "producer/network_producer.h"

int main() 
{
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);
#endif

    // create shared directory
    const std::string shared_directory = "network_shared_files";

    if (!std::filesystem::exists(shared_directory)) 
    {
        if (std::filesystem::create_directory(shared_directory))
            std::cout << "Created shared directory: " << shared_directory << "\n\n";
        else
        {
            std::cerr << "Failed to create a shared directory fo student XML files!\n";
            return 1;
        }
    }

    // create network buffer
    network_buffer net_buffer_(shared_directory);

    // create producer and server
    network_producer producer(net_buffer_);
    network_server server(net_buffer_, 8080);

    // initialize server
    if (!server.initialize()) 
    {
        std::cerr << "Failed to initialize server\n";
        return 1;
    }

    const int num_students = 10;

    // start producer thread
    std::thread producer_thread(&network_producer::run, &producer, num_students);

    // start server thread (waits for clients and sends data)
    std::thread server_thread(&network_server::run, &server);

    // wait for both threads
    producer_thread.join();
    server_thread.join();

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}