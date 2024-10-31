#include <iostream>
#include <Network/NetworkPrototype.h>

#include "Network/NetworkServer.h"

void Server();

int main(int argc, char* argv[])
{
    NetworkPrototype network;
    if(argc > 1 && strcmp(argv[1], "-client") == 0)
        network.Client();
    else
        Server();
}

void Server()
{
    std::optional<NetworkServer> server = NetworkServer::Create("643610");
    if(!server)
        return;

    server->Listen();
    std::cout << "Finished!" << std::endl;
}