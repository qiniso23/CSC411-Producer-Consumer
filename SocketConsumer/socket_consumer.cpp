#include "socket_consumer.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

//***************************************************************************************************************************************************
socket_consumer::socket_consumer(const std::string& _ip, int _port)
	: server_ip(_ip)
	, port(_port)
{}

socket_consumer::~socket_consumer()
{
#ifdef _WIN32
        WSACleanup();
#endif
}

//***************************************************************************************************************************************************
bool socket_consumer::initialize()
{
#ifdef _WIN32
	WSADATA wsa_data;
        
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		std::cerr << "WSAStartup failed\n";
        return false;
    }
#endif
    return true;
}

//***************************************************************************************************************************************************
void socket_consumer::consume_one()
{
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
	if (client_socket == INVALID_SOCKET)
	{
		std::cerr << "[CONSUMER] Socket creation failed\n";
        return;
    }
        
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);
        
    std::cout << "[CONSUMER] Connecting to producer...\n";

    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
        std::cerr << "[CONSUMER] Connection failed\n";
        closesocket(client_socket);
        return;
    }
        
    std::cout << "[CONSUMER] Connected to producer\n";
        
    // receive data
    char buffer[4096];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
    if (bytes_received > 0)
	{
        buffer[bytes_received] = '\0';

        std::string xml_data(buffer);
            
        std::cout << "[CONSUMER] Received " << bytes_received << " bytes\n";
            
        // parse XML
        ITstudent student = ITstudent::from_XML(xml_data);
            
        // display student info
        print_student_info(student);
    }
        
    closesocket(client_socket);
}

//***************************************************************************************************************************************************
void socket_consumer::run(int _num_students)
{
    std::cout << "[CONSUMER] Starting to consume " << _num_students << " students...\n";
        
    for (int i = 0; i < _num_students; i++) 
	{
        consume_one();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
        
    std::cout << "[CONSUMER] Finished consumption\n";
}

//***************************************************************************************************************************************************
void socket_consumer::print_student_info(const ITstudent& _student) const
{
    std::cout << "\n========================================\n";
    std::cout << "Student Name: " << _student.get_full_name() << "\n";
    std::cout << "Student ID: " << _student.get_student_id() << "\n";
    std::cout << "Programme: " << _student.get_programme() << "\n";
    std::cout << "Courses and Marks:\n";

    for (const auto& course_mark_ : _student.get_course_marks())
        std::cout << "  - " << course_mark_.course_code << ": " << course_mark_.mark << "\n";

    const float avg_mark = _student.calculate_average();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average Mark: " << avg_mark << "\n";
    std::cout << "Status: " << ((avg_mark > 50.0f) ? "PASSED" : "FAILED") << "\n";
    std::cout << "========================================\n";
}