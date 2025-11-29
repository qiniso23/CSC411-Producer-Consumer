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
#include <random>
#include <vector>

//***************************************************************************************************************************************************
class socket_producer
{
public:
	//***********************************************************************************************************************************************
    socket_producer(int _port);
    ~socket_producer();
    
    bool initialize();
    ITstudent generate_random_student();
    void run(int _num_students);

private:
	//********************************************************************************************************************************************
    SOCKET server_socket;
    int port;
    std::mt19937 generator;
    
    const std::vector<std::string> first_names = {"Sifiso", "Muzi", "James", "Pertunia", "Clementine", "Lindani", "Busiswa", "Barbara", "Elizabeth" };
    const std::vector<std::string> last_names = { "Smith", "Fakudze", "Xulu", "Shiba", "Jones", "Garcia", "Mnisi", "Davis", "Ngcamphalala", "Martinez" };
    const std::vector<std::string> programmes = { "BSc Computer Science", "BSc Information Technology", "BSc Software Engineering", "BSc Data Science" };
    const std::vector<std::string> course_codes = { "CSC231", "CSC123", "CSC456", "CSC321", "CSC333", "CSC411", "CSC312" };
};