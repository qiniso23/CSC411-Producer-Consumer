#include "producer.h"
#include "consumer.h"
#include <filesystem>
#include <iostream>

int main()
{
    std::string shared_dir = "shared_files";

    if (!std::filesystem::exists(shared_dir)) 
    {
        if(std::filesystem::create_directory(shared_dir))
            std::cout << "Created shared directory: " << shared_dir << "\n\n";
        else
        {
            std::cerr << "Failed to create a shared directory fo student XML files!\n";
            return 1;
        }
    }

    // create buffer
    buffer buffer_(shared_dir);

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