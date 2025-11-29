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

#include "ITstudent.h"

//***************************************************************************************************************************************************
class socket_consumer 
{
public:
	//***********************************************************************************************************************************************
    socket_consumer(const std::string& _ip, int _port);
    ~socket_consumer();
    
    bool initialize();
    void consume_one();
    void run(int _num_students);

private:
	//***********************************************************************************************************************************************
    std::string server_ip;
    int port;

	//***********************************************************************************************************************************************
	void print_student_info(const ITstudent& _student) const;
};