#pragma once
#include <optional>
#include <string_view>
#include <Winsock2.h>

// https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock
class NetworkClient
{
public:
    static std::optional<NetworkClient> Connect(std::string_view serverIP, std::string_view port);

    NetworkClient(const NetworkClient&) = delete;
    NetworkClient operator=(const NetworkClient&) = delete;

    NetworkClient(NetworkClient&& other) noexcept;
    NetworkClient& operator=(NetworkClient&& other) noexcept;

    //Temp functions
    void SendStuff();
    void ReceiveStuff();

    bool Receive(char* data, int size);
    
    bool IsConnected() const;

    ~NetworkClient();

private:
    // Use Connect to make a connection to a server
    NetworkClient(std::string_view serverIP, std::string_view port);

    bool isConnected = false;
    SOCKET socket = INVALID_SOCKET;
};
