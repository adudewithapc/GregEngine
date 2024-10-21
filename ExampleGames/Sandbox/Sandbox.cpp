#include <iostream>
#include <Network/Network.h>

int main(int argc, char* argv[])
{
    Network network;
    if(argc > 1 && strcmp(argv[1], "-client") == 0)
        network.Client();
    else
        network.Server();
}
