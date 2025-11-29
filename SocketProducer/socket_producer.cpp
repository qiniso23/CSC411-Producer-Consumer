#include "socket_producer.h"
#include <iostream>
#include <chrono>
#include <thread>

//***************************************************************************************************************************************************
socket_producer::socket_producer(int _port)
	: port(_port)
	, server_socket(INVALID_SOCKET) 
{
        std::random_device rd;
		generator.seed(rd());
}

//***************************************************************************************************************************************************    
socket_producer::~socket_producer()
{
	if (server_socket != INVALID_SOCKET)
		closesocket(server_socket);

#ifdef _WIN32
        WSACleanup();
#endif
}

//***************************************************************************************************************************************************
bool socket_producer::initialize() 
{
#ifdef _WIN32
	WSADATA wsa_data;
        
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		std::cerr << "WSAStartup failed\n";
        return false;
    }
#endif
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
        
	if (server_socket == INVALID_SOCKET) 
	{
		std::cerr << "Socket creation failed\n";
        return false;
    }
        
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
        
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) 
	{
        std::cerr << "Bind failed\n";
        return false;
    }
        
    if (listen(server_socket, 5) == SOCKET_ERROR) 
	{
        std::cerr << "Listen failed\n";
        return false;
    }
        
    std::cout << "[PRODUCER SERVER] Listening on port " << port << "...\n";
    return true;
}

//***************************************************************************************************************************************************
ITstudent socket_producer::generate_random_student() 
{
	std::uniform_int_distribution<int> first_dist(0, first_names.size() - 1);
    std::uniform_int_distribution<int> last_dist(0, last_names.size() - 1);
    std::uniform_int_distribution<int> id_dist(10000000, 99999999);
    std::uniform_int_distribution<int> prog_dist(0, programmes.size() - 1);
    std::uniform_int_distribution<int> num_courses(4, 6); // assume a student can take a range [4, 6] course
	std::uniform_real_distribution<float> mark(0.0f, 100.0f);
	std::vector<std::string> shuffled_cms = course_codes;
    
	std::string first_name = first_names[first_dist(generator)];
	std::string last_name =  last_names[last_dist(generator)];
	int id = id_dist(generator);
	std::string programme = programmes[prog_dist(generator)];
        
	std::vector<course_mark> course_marks;

	std::shuffle(shuffled_cms.begin(), shuffled_cms.end(), generator);

    const int N = std::min((int)shuffled_cms.size(), num_courses(generator));

    for (int i = 0; i < N; i++)
        course_marks.push_back({ shuffled_cms[i], mark(generator) });
        
	return ITstudent(first_name, last_name, id, programme, course_marks);
}

//***************************************************************************************************************************************************
void socket_producer::run(int _num_students) 
{
	for (int i = 0; i < _num_students; i++) 
	{
		std::cout << "\n[PRODUCER SERVER] Waiting for consumer connection...\n";
            
        SOCKET client_socket = accept(server_socket, nullptr, nullptr);
		
		if (client_socket == INVALID_SOCKET)
		{
			std::cerr << "[PRODUCER SERVER] Accept failed\n";
            continue;
        }
            
        std::cout << "[PRODUCER SERVER] Consumer connected\n";
            
        // generate student
        ITstudent student = generate_random_student();
        std::string xml_data = student.to_XML();
            
        // send XML data
        send(client_socket, xml_data.c_str(), xml_data.length(), 0);
        std::cout << "[PRODUCER SERVER] Sent student data (" << xml_data.length() << " bytes)\n";
        closesocket(client_socket);
        std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}