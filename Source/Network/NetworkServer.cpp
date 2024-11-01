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

void NetworkServer::ListenForStuff()
{
    std::cout << "Waiting for client 1..." << std::endl;
    SOCKET senderSocket = WaitForConnection();
    if(senderSocket == INVALID_SOCKET)
    {
        std::cerr << "Client 1 failed to connect. Error code: " << WSAGetLastError() << std::endl;
        return;
    }
    
    std::cout << "Client 1 connected!" << std::endl;

    std::cout << "Waiting for client 2..." << std::endl;
    SOCKET receiverSocket = WaitForConnection();
    if(receiverSocket == INVALID_SOCKET)
    {
        std::cerr << "Client 2 failed to connect. Error code: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "Client 2 connected!" << std::endl;

    if(!Send(senderSocket, "sender", 1024) || !Send(receiverSocket, "receiver", 1024))
    {
        std::cerr << "Could not send stuff :(" << std::endl;
        return;
    }
    
    std::cout << "Listening..." << std::endl;
    while(true)
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        if(!Receive(senderSocket, buffer, 1024))
        {
            break;
        }

        if(!Send(receiverSocket, buffer, 1024))
        {
            break;
        }

        std::cout << "Received and sent: " << buffer << std::endl;
        
        if(strcmp(buffer, "exit") == 0)
            break;
    }
}

bool NetworkServer::IsOpen() const
{
    return isOpen;
}

bool NetworkServer::Send(SOCKET receiver, const char* data, int size)
{
    if(!isOpen)
    {
        std::cerr << "Tried to send data when the server is not open!" << std::endl;
        return false;
    }
    
    if(send(receiver, data, size, 0) == SOCKET_ERROR)
    {
        std::cerr << "Failed to send data. Error code: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

bool NetworkServer::Receive(SOCKET sender, char* data, int size)
{
    if(!isOpen)
    {
        std::cerr << "Tried tor receive data when the server is not open!" << std::endl;
        return false;
    }

    if(recv(sender, data, size, 0) == SOCKET_ERROR)
    {
        std::cerr << "Failed to receive data. Error code: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

SOCKET NetworkServer::WaitForConnection()
{
    if(listen(socket, 1) == SOCKET_ERROR)
    {
        std::cerr << "Listening on socket failed. Error code: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }
    
    return accept(socket, nullptr, nullptr);
}

NetworkServer::~NetworkServer()
{
    if(socket == INVALID_SOCKET)
        return;
    
    closesocket(socket);
    WSACleanup();
}
