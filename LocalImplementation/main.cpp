#include <iostream>
#include <filesystem>
#include "producer/producer.h"
#include "consumer/consumer.h"

int main()
{
    std::string shared_directory = "shared_files";

    if (!std::filesystem::exists(shared_directory)) 
    {
        if(std::filesystem::create_directory(shared_directory))
            std::cout << "Created shared directory: " << shared_directory << "\n\n";
        else
        {
            std::cerr << "Failed to create a shared directory fo student XML files!\n";
            return 1;
        }
    }

    // create buffer
    buffer buffer_(shared_directory);

    // create producer and consumer
    producer producer_(buffer_);
    consumer consumer_(buffer_);

    // create producer and consumer threads
    int num_students = 10;
    std::thread producer_thread(&producer::run, &producer_, num_students);
    std::thread consumer_thread(&consumer::run, &consumer_, num_students);

    // wait for both threads to complete
    producer_thread.join();
    consumer_thread.join();

	return 0;
}