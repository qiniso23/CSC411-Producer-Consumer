#pragma once

// the buffer we will use will be a FIFO
#include <queue>
#include <mutex>
#include <string>
#include <condition_variable>

//***************************************************************************************************************************************************
class buffer 
{
public:
    buffer(const std::string& _directory);

    virtual void produce(int _file_number);
    virtual int consume();

    int size() const;
    bool is_full() const;
    bool is_empty() const;

    std::string get_shared_directory() const;

protected:
    //***********************************************************************************************************************************************
    std::mutex mtx;
    std::queue<int> buffer_;
    std::condition_variable not_full, not_empty;

private:
    //***********************************************************************************************************************************************
    std::string shared_directory;
    static constexpr int MAX_SIZE = 10;
};