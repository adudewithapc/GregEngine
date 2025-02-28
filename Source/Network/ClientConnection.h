#pragma once

#include <Winsock2.h>

class ClientConnection
{
public:
    ClientConnection(const SOCKET socket);

    bool Receive(char* buffer, int bufferSize);
    bool Send(const char* buffer, int bufferSize);
    
    ~ClientConnection();
    
private:
    SOCKET socket = INVALID_SOCKET;
};
