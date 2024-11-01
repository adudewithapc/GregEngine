#pragma once
#include <optional>
#include <string_view>
#include <Winsock2.h>

class NetworkServer
{
public:
    static std::optional<NetworkServer> Create(std::string_view port);

    NetworkServer(const NetworkServer&) = delete;
    NetworkServer operator =(const NetworkServer&) = delete;

    NetworkServer(NetworkServer&& other);
    NetworkServer& operator =(NetworkServer&& other);

    //Temp function
    void ListenForStuff();
    
    bool IsOpen() const;

    bool Send(SOCKET receiver, const char* data, int size);

    ~NetworkServer();
private:
    // Use Create to start
    NetworkServer(std::string_view port);

    SOCKET WaitForConnection();
    
    bool isOpen = false;
    SOCKET socket = INVALID_SOCKET;
};
