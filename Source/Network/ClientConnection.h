#pragma once

#include <Winsock2.h>

class ClientConnection
{
public:
    ClientConnection(const ClientConnection&) = delete;
    ClientConnection operator =(const ClientConnection&) = delete;

    ClientConnection(ClientConnection&& other) = delete;
    ClientConnection& operator =(ClientConnection&& other) = delete;

    bool Receive(char* buffer, int bufferSize);
    bool Send(const char* buffer, int bufferSize);
    
    ~ClientConnection();
private:
    ClientConnection(const SOCKET socket);

    SOCKET socket = INVALID_SOCKET;
};
