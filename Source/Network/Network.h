#pragma once

class Network
{
public:
    Network();
    ~Network();
    
    void Server();
    void Client();

private:
    void Cleanup(unsigned long long socket);
};
