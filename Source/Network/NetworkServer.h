#pragma once
#include <optional>
#include <string_view>
#include <vector>
#include <Winsock2.h>

class ClientConnection;

// https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock
class NetworkServer
{
public:
    static std::optional<NetworkServer> Create(std::string_view port);

    NetworkServer(const NetworkServer&) = delete;
    NetworkServer operator =(const NetworkServer&) = delete;

    NetworkServer(NetworkServer&& other) noexcept;
    NetworkServer& operator =(NetworkServer&& other) noexcept;
    
    bool Send(SOCKET receiverConnection, const char* data, int size);
    bool Receive(SOCKET senderConnection, char* data, int size);

    int WaitForConnection();

    bool IsOpen() const;
    
    ~NetworkServer();

    friend class ClientConnection;
private:
    // Use Create to start
    NetworkServer(std::string_view port);

    std::vector<ClientConnection> connections;
    bool isOpen = false;
    SOCKET socket = INVALID_SOCKET;
};
