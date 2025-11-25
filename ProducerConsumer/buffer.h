#pragma once

// the buffer we will use will be a FIFO
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

//***************************************************************************************************************************************************
class buffer 
{
public:
    buffer(const std::string& _directory);

    void produce(int _file_number);
    int consume();

    int size() const;
    bool is_full() const;
    bool is_empty() const;

    std::string get_shared_directory() const;

private:
    //***********************************************************************************************************************************************
    static constexpr int MAX_SIZE = 10;
    std::queue<int> buffer_;
    std::mutex mtx;
    std::condition_variable not_full;
    std::condition_variable not_empty;
    std::string shared_directory;
};