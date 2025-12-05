#include "network_buffer.h"
#include <iostream>

//***************************************************************************************************************************************************
network_buffer::network_buffer(const std::string& _directory)
	: buffer(_directory)
	, finished(false)
{}

//***************************************************************************************************************************************************
int network_buffer::consume()
{
    std::unique_lock<std::mutex> lock(mtx);

    // wait while buffer is empty
    not_empty.wait(lock, [this]() { return !buffer_.empty() || finished; });

    if (buffer_.empty() && finished)
        return -1; // signal end

    int file_number = buffer_.front();

    buffer_.pop();
    std::cout << "[NETWORK BUFFER] Consumed: " << file_number << " (new buffer size: " << buffer_.size() << ")\n";

    // notify producer that buffer is not full
    not_full.notify_one();

    return file_number;
}

//***************************************************************************************************************************************************
void network_buffer::set_finished()
{
    std::unique_lock<std::mutex> lock(mtx);

    finished = true;
    not_empty.notify_all();
}