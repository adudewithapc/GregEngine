#include "NetworkServer.h"

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

std::optional<NetworkServer> NetworkServer::Create(std::string_view port)
{
    NetworkServer server(port);

    if(!server.IsOpen())
        return {};

    return std::move(server);
}

NetworkServer::NetworkServer(std::string_view port)
{
    WORD versionRequested = MAKEWORD(2, 2);
    WSAData wsaData;
    if(WSAStartup(versionRequested, &wsaData))
    {
        return;
    }

    struct addrinfo *result = nullptr, *ptr = nullptr, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int addressResult = getaddrinfo(nullptr, port.data(), &hints, &result);

    if(addressResult != 0)
    {
        std::cerr << "Failed to fetch address info. Error code: " << addressResult << std::endl;
        return;
    }

    socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if(socket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed. Error code: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        return;
    }

    if(bind(socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
    {
        std::cerr << "Listening on socket failed. Error code: " << WSAGetLastError() << std::endl;
        return;
    }

    isOpen = true;

    //freeaddrinfo(result);
    
    // Server server = NetworkHandler::startServer();
    // networ handler checks if valid
    // asdfdsaf
    // out of scope, close connection
}

NetworkServer::NetworkServer(NetworkServer&& other)
{
    isOpen = other.isOpen;
    socket = other.socket;

    other.isOpen = false;
    other.socket = INVALID_SOCKET;
}

NetworkServer& NetworkServer::operator=(NetworkServer&& other)
{
    isOpen = other.isOpen;
    socket = other.socket;

    other.isOpen = false;
    other.socket = INVALID_SOCKET;

    return *this;
}

void NetworkServer::Listen()
{
    if(listen(socket, 2) == SOCKET_ERROR)
    {
        std::cerr << "Listening on socket failed. Error code: " << WSAGetLastError() << std::endl;
        return;
    }
    
    SOCKET clientSocket = accept(socket, nullptr, nullptr);

    if(clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Client failed to connect. Error code: " << WSAGetLastError() << std::endl;
        return;
    }
    
    std::cout << "Connected!" << std::endl;
    
    char buffer[1024];
    std::cout << "Listening..." << std::endl;
    
    while(true)
    {
        memset(buffer, 0, sizeof(buffer));
        if(recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
        {
            std::cerr << "Error when receiving data. Error code: " << WSAGetLastError() << std::endl;
            break;
        }

        std::cout << "Received: " << buffer << std::endl;
        
        if(strcmp(buffer, "exit") == 0)
            break;
    }
}

bool NetworkServer::IsOpen() const
{
    return isOpen;
}

NetworkServer::~NetworkServer()
{
    if(socket == INVALID_SOCKET)
        return;
    
    closesocket(socket);
    WSACleanup();
}
