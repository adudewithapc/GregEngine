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

    void Listen();
    bool IsOpen() const;

    ~NetworkServer();
private:
    // Use Create to start
    NetworkServer(std::string_view port);
    
    bool isOpen = false;
    SOCKET socket = INVALID_SOCKET;
};
