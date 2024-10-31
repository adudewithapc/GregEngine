#include "NetworkPrototype.h"

#include <iostream>
#include <ostream>
#include <Winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_PORT "643610"

NetworkPrototype::NetworkPrototype()
{
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    if(WSAStartup(wVersionRequested, &wsaData))
    {
        std::cerr << "Error running WSAStartup." << std::endl;
    }
}

NetworkPrototype::~NetworkPrototype()
{
    WSACleanup();
}

void NetworkPrototype::Server()
{
    struct addrinfo *result = nullptr, *ptr = nullptr, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int addressResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);

    if(addressResult != 0)
    {
        std::cerr << "Failed to fetch address info. Error code: " << addressResult << std::endl;
        WSACleanup();
        return;
    }
    
    SOCKET serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if(serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed. Error code: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        Cleanup(serverSocket);
        return;
    }
    
    if(bind(serverSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
    {
        std::cerr << "Socket bind failed. Error code:" << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        Cleanup(serverSocket);
        return;
    }

    if(listen(serverSocket, 1) == SOCKET_ERROR)
    {
        std::cerr << "Listening on socket failed. Error code: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "Listening on server..." << std::endl;
    
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    char buffer[1024];

    while(true)
    {
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer << std::endl;
        if(strcmp(buffer, "exit") == 0)
            break;
    }

    freeaddrinfo(result);
    Cleanup(serverSocket);
}

void NetworkPrototype::Client()
{
    struct addrinfo *result = nullptr, *ptr = nullptr, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int addrInfoResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);

    if(addrInfoResult != 0)
    {
        std::cerr << "Failed to fetch address info. Error code: " << addrInfoResult << std::endl;
        WSACleanup();
        return;
    }

    ptr = result;

    SOCKET clientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if(clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Creating client socket failed. Error code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    int connectionResult = connect(clientSocket, ptr->ai_addr, ptr->ai_addrlen);
    if(connectionResult == SOCKET_ERROR)
    {
        std::cerr << "Client connection failed. Error code: " << WSAGetLastError() << std::endl;
        Cleanup(clientSocket);
        return;
    }

    char message[1024];
    while(true)
    {
        std::cout << "Enter the message to send: ";
        std::cin.getline(message, 1024);
        send(clientSocket, message, sizeof(message), 0);
        if(strcmp(message, "exit\n") == 0)
            break;
        memset(message, 0, sizeof(message));
    }
    closesocket(clientSocket);
}

void NetworkPrototype::Cleanup(unsigned long long socket)
{
    closesocket(socket);
    WSACleanup();
}
