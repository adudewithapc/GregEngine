#include "ClientConnection.h"

#include <iostream>

bool ClientConnection::Receive(char* buffer, int bufferSize)
{
    if(recv(socket, buffer, bufferSize, 0) == SOCKET_ERROR)
    {
        std::cerr << "Error receiving buffer. Error code: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

bool ClientConnection::Send(const char* buffer, int bufferSize)
{
    if(send(socket, buffer, bufferSize, 0) == SOCKET_ERROR)
    {
        std::cerr << "Error sending buffer. Error code: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

ClientConnection::~ClientConnection()
{
    if (socket != INVALID_SOCKET)
        closesocket(socket);
}

ClientConnection::ClientConnection(const SOCKET socket)
    : socket(socket)
{}
