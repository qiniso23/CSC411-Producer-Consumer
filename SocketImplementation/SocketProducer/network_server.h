#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

#include "buffer/network_buffer.h"

// server thread - sends data to clients over network
//***************************************************************************************************************************************************
class network_server 
{
public:
    //***********************************************************************************************************************************************
    network_server(network_buffer& _buffer, int _port);
    ~network_server();

    bool initialize();
    void run();

private:
    //***********************************************************************************************************************************************
    network_buffer& net_buffer_;
    SOCKET server_socket;
    int port;
};