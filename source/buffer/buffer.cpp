#include "buffer.h"
#include <iostream>

//***************************************************************************************************************************************************
buffer::buffer(const std::string& _directory)
    : shared_directory(_directory) 
{}

//***************************************************************************************************************************************************
void buffer::produce(int _file_number) 
{
    std::unique_lock<std::mutex> lock(mtx);

    // wait while buffer is full
    not_full.wait(lock, [this]() { return buffer_.size() < MAX_SIZE; });

    buffer_.push(_file_number);
    std::cout << "[BUFFER] Produced: " << _file_number << " (new buffer size: " << buffer_.size() << ")\n";

    // notify consumer that buffer is not empty
    not_empty.notify_one();
}

//***************************************************************************************************************************************************
int buffer::consume() 
{
    std::unique_lock<std::mutex> lock(mtx);

    // wait while buffer is empty
    not_empty.wait(lock, [this]() { return !buffer_.empty(); });

    int file_number = buffer_.front();

    buffer_.pop();
    std::cout << "[BUFFER] Consumed: " << file_number << " (new buffer size: " << buffer_.size() << ")\n";

    // notify producer that buffer is not full
    not_full.notify_one();

    return file_number;
}

//***************************************************************************************************************************************************
int buffer::size() const
{
    return (int)buffer_.size();
}

//***************************************************************************************************************************************************
bool buffer::is_full() const 
{
    return buffer_.size() >= MAX_SIZE;
}

//***************************************************************************************************************************************************
bool buffer::is_empty() const 
{
    return buffer_.empty();
}

//***************************************************************************************************************************************************
std::string buffer::get_shared_directory() const
{
    return shared_directory;
}