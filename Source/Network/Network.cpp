#include "Network.h"

#include <iostream>
#include <ostream>
#include <winsock.h>

Network::Network()
{
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    if(WSAStartup(wVersionRequested, &wsaData))
    {
        std::cerr << "Error running WSAStartup." << std::endl;
    }
}

Network::~Network()
{
    WSACleanup();
}

void Network::Server()
{
    unsigned long long serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed. Error code: " << WSAGetLastError() << std::endl;
        Cleanup(serverSocket);
        return;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(27015);

    if(bind(serverSocket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
    {
        std::cerr << "Socket bind failed. Error code:" << WSAGetLastError() << std::endl;
        Cleanup(serverSocket);
        return;
    }

    if(listen(serverSocket, 1) == -1)
    {
        std::cerr << "Listening on socket failed. Error code: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "Listening on server..." << std::endl;
    
    unsigned long long clientSocket = accept(serverSocket, nullptr, nullptr);
    char buffer[1024];

    while(true)
    {
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer << "\n";
        if(strcmp(buffer, "exit\n") == 0)
            break;
    }

    Cleanup(serverSocket);
}

void Network::Client()
{
    unsigned long long clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Error creating client socket. Error code: " << WSAGetLastError() << std::endl;
        Cleanup(clientSocket);
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(27015);

    int connectionResult = connect(clientSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
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
        std::cin >> message;
        send(clientSocket, message, sizeof(message), 0);
        if(strcmp(message, "exit\n"))
            break;
        memset(message, 0, sizeof(message));
    }
    closesocket(clientSocket);
}

void Network::Cleanup(unsigned long long socket)
{
    closesocket(socket);
    WSACleanup();
}
