#pragma once

class NetworkPrototype
{
public:
    NetworkPrototype();
    ~NetworkPrototype();
    
    void Server();
    void Client();

private:
    void Cleanup(unsigned long long socket);
};
