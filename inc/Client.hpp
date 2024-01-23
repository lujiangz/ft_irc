#pragma once

#include "Server.hpp"

enum RegState
{
    None,
    PassReg,
    NickReg,
    UserReg,
};

class Client
{
    private:
        int _socket;        
    public:
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _hostname;
        std::string _invChan;

        enum RegState _status;
        bool _online;
        
        int getSocketFd() const;
        Client(int socketFd);

        void   checkHostname(sockaddr_in serverAddr);
};