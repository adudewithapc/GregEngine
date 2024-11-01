#include <iostream>

#include "Network/NetworkClient.h"
#include "Network/NetworkServer.h"

void Server();
void Client();

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

    server->ListenForStuff();
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
        client->SendStuff();
    else if(strcmp("receiver", data) == 0)
        client->ReceiveStuff();
    else
        std::cerr << "Received neither \"sender\" nor \"receiver\". Instead received " << data << std::endl;
    
    std::cout << "Finished!" << std::endl;
}
