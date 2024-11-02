#include <iostream>

#include "Network/NetworkClient.h"
#include "Network/NetworkServer.h"

void Server();
void DistributeStuff(NetworkServer& server);

void Client();
void SendStuff(NetworkClient& client);
void ReceiveStuff(NetworkClient& client);

int main(int argc, char* argv[])
{
    if(argc > 1 && strcmp(argv[1], "-client") == 0)
        Client();
    else
        Server();
}

void Server()
{
    std::optional<NetworkServer> server = NetworkServer::Create("643610");
    
    if(!server)
        return;

    DistributeStuff(*server);
    
    std::cout << "Finished!" << std::endl;
}

void Client()
{
    std::optional<NetworkClient> client = NetworkClient::Connect("127.0.0.1", "643610");

    if(!client)
        return;

    char* data = new char[1024];
    if(!client->Receive(data, 1024))
        return;
    
    if(strcmp("sender", data) == 0)
        SendStuff(*client);
    else if(strcmp("receiver", data) == 0)
        ReceiveStuff(*client);
    else
        std::cerr << "Received neither \"sender\" nor \"receiver\". Instead received " << data << std::endl;
    
    std::cout << "Finished!" << std::endl;
}

void DistributeStuff(NetworkServer& server)
{
    std::cout << "Waiting for client 1..." << std::endl;
    int senderConnection = server.WaitForConnection();
    if(senderConnection == -1)
    {
        std::cerr << "Client 1 failed to connect. Error code: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "Client 1 connected!\nWaiting for client 2..." << std::endl;

    int receiverConnection = server.WaitForConnection();
    if(receiverConnection == -1)
    {
        std::cerr << "Client 2 failed to connect. Error code: " << WSAGetLastError() << std::endl;
        return;
    }

    std::cout << "Client 2 connected!" << std::endl;

    if(!server.Send(senderConnection, "sender", 1024) || !server.Send(receiverConnection, "receiver", 1024))
    {
        std::cerr << "Could not send sender or receiver flags." << std::endl;
        return;
    }

    std::cout << "Listening..." << std::endl;
    while(true)
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        if(!server.Receive(senderConnection, buffer, 1024))
        {
            break;
        }

        if(!server.Send(receiverConnection, buffer, 1024))
        {
            break;
        }

        std::cout << "Received and sent: " << buffer << std::endl;

        if(strcmp(buffer, "exit") == 0)
            break;
    }
}

void SendStuff(NetworkClient& client)
{
    char message[1024];
    while(true)
    {
        std::cout << "Enter the message to send: ";
        std::cin.getline(message, 1024);
        client.Send(message, sizeof(message));

        if(strcmp(message, "exit") == 0)
            break;

        memset(message, 0, sizeof(message));
    }
}

void ReceiveStuff(NetworkClient& client)
{
    while(true)
    {
        char data[1024];
        if(!client.Receive(data, 1024))
        {
            std::cerr << "Couldn't receive data. Error code: " << WSAGetLastError() << std::endl;
            return;
        }

        std::cout << "Other client: " << data << std::endl;

        if(strcmp(data, "exit") == 0)
            break;
    }
}
