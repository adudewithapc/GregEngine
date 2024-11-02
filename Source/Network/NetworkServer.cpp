#include "NetworkServer.h"

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "ClientConnection.h"

std::optional<NetworkServer> NetworkServer::Create(std::string_view port)
{
    NetworkServer server(port);

    if(!server.isOpen)
        return {};

    return server;
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

NetworkServer::NetworkServer(NetworkServer&& other) noexcept
{
    isOpen = other.isOpen;
    socket = other.socket;

    other.isOpen = false;
    other.socket = INVALID_SOCKET;
}

NetworkServer& NetworkServer::operator=(NetworkServer&& other) noexcept
{
    isOpen = other.isOpen;
    socket = other.socket;

    other.isOpen = false;
    other.socket = INVALID_SOCKET;

    return *this;
}

bool NetworkServer::IsOpen() const
{
    return isOpen;
}

bool NetworkServer::Send(size_t receiverConnection, const char* data, int size)
{
    if(!isOpen)
    {
        std::cerr << "Tried to send data when the server is not open!" << std::endl;
        return false;
    }
    
    return (connections.begin() + receiverConnection)->Send(data, size);
}

bool NetworkServer::Receive(size_t senderConnection, char* data, int size)
{
    if(!isOpen)
    {
        std::cerr << "Tried tor receive data when the server is not open!" << std::endl;
        return false;
    }

    return (connections.begin() + senderConnection)->Receive(data, size);
}

int NetworkServer::WaitForConnection()
{
    if(listen(socket, 1) == SOCKET_ERROR)
    {
        std::cerr << "Listening on socket failed. Error code: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

    SOCKET newConnection = accept(socket, nullptr, nullptr);

    if(newConnection == INVALID_SOCKET)
        return -1;

    connections.emplace_back(newConnection);
    return connections.size() - 1;
}

NetworkServer::~NetworkServer()
{
    if(socket == INVALID_SOCKET)
        return;
    
    closesocket(socket);
    WSACleanup();
}
