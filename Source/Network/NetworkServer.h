#pragma once
#include <optional>
#include <string_view>
#include <Winsock2.h>

// https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock
class NetworkServer
{
public:
    static std::optional<NetworkServer> Create(std::string_view port);

    NetworkServer(const NetworkServer&) = delete;
    NetworkServer operator =(const NetworkServer&) = delete;

    NetworkServer(NetworkServer&& other) noexcept;
    NetworkServer& operator =(NetworkServer&& other) noexcept;
    
    bool Send(SOCKET receiver, const char* data, int size);
    bool Receive(SOCKET sender, char* data, int size);

    SOCKET WaitForConnection();

    bool IsOpen() const;
    
    ~NetworkServer();
private:
    // Use Create to start
    NetworkServer(std::string_view port);
    
    bool isOpen = false;
    SOCKET socket = INVALID_SOCKET;
};
