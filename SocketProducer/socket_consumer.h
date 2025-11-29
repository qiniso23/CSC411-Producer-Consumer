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

#include "ITStudent.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

class SocketConsumer {
private:
    std::string serverIP;
    int port;

public:
    SocketConsumer(const std::string& ip, int p) : serverIP(ip), port(p) {}
    
    ~SocketConsumer() {
#ifdef _WIN32
        WSACleanup();
#endif
    }
    
    bool initialize() {
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return false;
        }
#endif
        return true;
    }
    
    void consumeOne() {
        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "[CONSUMER] Socket creation failed\n";
            return;
        }
        
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);
        
        std::cout << "[CONSUMER] Connecting to producer...\n";
        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "[CONSUMER] Connection failed\n";
            closesocket(clientSocket);
            return;
        }
        
        std::cout << "[CONSUMER] Connected to producer\n";
        
        // Receive data
        char buffer[4096];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string xmlData(buffer);
            
            std::cout << "[CONSUMER] Received " << bytesReceived << " bytes\n";
            
            // Parse XML
            ITStudent student = ITStudent::fromXML(xmlData);
            
            // Display student info
            student.printStudentInfo();
        }
        
        closesocket(clientSocket);
    }
    
    void run(int numStudents) {
        std::cout << "[CONSUMER] Starting to consume " << numStudents << " students...\n";
        
        for (int i = 0; i < numStudents; i++) {
            consumeOne();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        
        std::cout << "[CONSUMER] Finished consumption\n";
    }
};

int main() {
    SocketConsumer consumer("127.0.0.1", 8080);
    
    if (!consumer.initialize()) {
        return 1;
    }
    
    std::cout << "Consumer client starting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for server
    
    consumer.run(10); // Consume 10 students
    
    return 0;
}