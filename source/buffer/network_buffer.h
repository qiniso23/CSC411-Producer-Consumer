#pragma once

#include "buffer.h"

// Network Buffer - shared between producer and network sender threads

//***************************************************************************************************************************************************
class network_buffer : public buffer
{
public:
    //***********************************************************************************************************************************************
    network_buffer(const std::string& _directory);

    int consume() override;
    void set_finished();

private:
    //***********************************************************************************************************************************************
    bool finished;
};