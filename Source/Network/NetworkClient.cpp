#include "NetworkClient.h"

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

std::optional<NetworkClient> NetworkClient::Connect(std::string_view serverIP, std::string_view port)
{
    NetworkClient client(serverIP, port);

    if(!client.IsConnected())
        return {};

    return std::move(client);
}

NetworkClient::NetworkClient(std::string_view serverIP, std::string_view port)
{
    WORD versionRequested = MAKEWORD(2, 2);
    WSAData wsaData;

    if(WSAStartup(versionRequested, &wsaData) != 0)
    {
        std::cerr << "Error running WSAStartup." << std::endl;
        return;
    }
    
    struct addrinfo *result = nullptr, *ptr = nullptr, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int addrInfoResult = getaddrinfo(serverIP.data(), port.data(), &hints, &result);

    if(addrInfoResult != 0)
    {
        std::cerr << "Failed to fetch address info. Error code: " << addrInfoResult << std::endl;
        return;
    }

    socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    for(ptr = result; ptr != nullptr; ptr = ptr->ai_next)
    {
        if(socket == INVALID_SOCKET)
        {
            std::cerr << "Socket creation failed. Error code: " << WSAGetLastError() << std::endl;
            return;
        }

        if(connect(socket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
        {
            std::cerr << "Client connection failed. Error code: " << WSAGetLastError() << std::endl;
            return;
        }
    }

    isConnected = true;
}

NetworkClient::NetworkClient(NetworkClient&& other) noexcept
{
    isConnected = other.isConnected;
    socket = other.socket;

    other.socket = INVALID_SOCKET;
}

NetworkClient& NetworkClient::operator=(NetworkClient&& other) noexcept
{
    isConnected = other.isConnected;
    socket = other.socket;

    other.socket = INVALID_SOCKET;

    return *this;
}

void NetworkClient::SendStuff()
{
    char message[1024];
    while(true)
    {
        std::cout << "Enter the message to send: ";
        std::cin.getline(message, 1024);
        send(socket, message, sizeof(message), 0);

        if(strcmp(message, "exit\n") == 0)
            break;

        memset(message, 0, sizeof(message));
    }
}

void NetworkClient::ReceiveStuff()
{
    while(true)
    {
        char data[1024];
        if(!Receive(data, 1024))
        {
            std::cerr << "Couldn't receive data :( Error code: " << WSAGetLastError() << std::endl; 
            return;
        }

        std::cout << "Person 1: " << data << std::endl;

        if(strcmp(data, "exit") == 0)
            break;
    }
}

bool NetworkClient::Receive(char* data, int size)
{
    if(!isConnected)
    {
        std::cerr << "Cannot receive data when the client is not connected!" << std::endl;
        return false;
    }

    if(recv(socket, data, size, 0) == SOCKET_ERROR)
    {
        std::cerr << "Receiving data failed. Error code: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

bool NetworkClient::IsConnected() const
{
    return isConnected;
}

NetworkClient::~NetworkClient()
{
    if(socket == INVALID_SOCKET)
        return;
    
    closesocket(socket);
    WSACleanup();
}