#include "../inc/Client.hpp"

Client::Client(int clientSocket) : _nickname(""), _username(""), _realname(""), _hostname("unknown"), _invChan(""), _status(None) , _online(true)
{
    _socket = clientSocket;
}

Client::~Client()
{
    close(_socket);
}

int Client::getSocketFd() const
{
    return _socket;
}

void    Client::checkHostname(sockaddr_in serverAddr)
{
    char hostname[1024];

    if (getnameinfo((struct sockaddr *) &serverAddr, sizeof(serverAddr), hostname, 1024, NULL, 0, NI_NUMERICSERV) != 0)
    {
        std::cerr << "Error: failed to get client hostname!"<< std::endl;
        return;
    }
    _hostname = std::string(hostname);
}