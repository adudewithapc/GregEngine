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

    NetworkServer(NetworkServer&&) = default;
    NetworkServer& operator =(NetworkServer&&) = default;

    void Listen();
    bool IsValid() const;

    ~NetworkServer();
private:
    // Use CreateServer to make a connection
    NetworkServer(std::string_view port);
    
    bool isValid = false;
    SOCKET socket = INVALID_SOCKET;
};
