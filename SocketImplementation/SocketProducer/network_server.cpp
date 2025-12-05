#include "network_server.h"
#include <fstream>
#include <iostream>
#include <filesystem>

//***************************************************************************************************************************************************
network_server::network_server(network_buffer& _buffer, int _port)
	: net_buffer_(_buffer)
	, server_socket(INVALID_SOCKET)
    , port(_port)
{}

network_server::~network_server()
{
    if (server_socket != INVALID_SOCKET)
        closesocket(server_socket);
}

//***************************************************************************************************************************************************
bool network_server::initialize()
{
#ifdef _WIN32
    WSADATA wsa_data;
    
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) 
    {
        std::cerr << "[SERVER] WSAStartup failed\n";
        return false;
    }
#endif

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET)
    {
        std::cerr << "[SERVER] Socket creation failed\n";
        return false;
    }

    // allow port reuse
    int opt = 1;

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) 
    {
        std::cerr << "[SERVER] Bind failed\n";
        return false;
    }

    if (listen(server_socket, 5) == SOCKET_ERROR) 
    {
        std::cerr << "[SERVER] Listen failed\n";
        return false;
    }

    std::cout << "[SERVER] Listening on port " << port << "...\n\n";
    return true;
}

//***************************************************************************************************************************************************
void network_server::run()
{
    std::cout << "[SERVER] Waiting to send data from buffer...\n";

    while (true) 
    {
        // consume from buffer
        int file_number = net_buffer_.consume();

        if (file_number == -1) 
        {
            std::cout << "[SERVER] No more data to send. Exiting.\n";
            break;
        }

        // read XML file
        std::string filename = net_buffer_.get_shared_directory() + "/student" + std::to_string(file_number) + ".xml";
        std::ifstream in_file(filename);

        if (!in_file.is_open())
        {
            std::cerr << "[SERVER] Error: Could not open file " << filename << "\n";
            continue;
        }

        std::string xml_content((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());

        in_file.close();

        // wait for client connection
        std::cout << "\n[SERVER] Waiting for consumer connection...\n\n";

        SOCKET client_socket = accept(server_socket, nullptr, nullptr);

        if (client_socket == INVALID_SOCKET) 
        {
            std::cerr << "[SERVER] Accept failed\n";
            continue;
        }

        std::cout << "[SERVER] Consumer connected\n";

        // send XML data
        send(client_socket, xml_content.c_str(), (int)xml_content.length(), 0);
        std::cout << "[SERVER] Sent student data (" << xml_content.length() << " bytes)\n";
        closesocket(client_socket);

        // delete the file after sending
        if (std::filesystem::remove(filename))
            std::cout << "[SERVER] Deleted file: " << filename << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}