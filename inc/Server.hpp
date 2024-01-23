#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../inc/Client.hpp"
#include <sys/types.h>
#include <fcntl.h>
#include <netdb.h>
#include "../inc/Utils.hpp"

class Server
{
    private:
        int _port;
        std::string _password;
        std::map<std::string, class Channel*> _channels;
        std::vector<class Client*> _clients;
        int _socketFd;
        sockaddr_in _serverAddr;
        
    public:
        Server(const std::string &Port, const std::string &Password);
        ~Server();
        void    setSocket();
        void    serverRun();
        void    readData(fd_set readfds);
        void    CmdMap();
        const   std::map<std::string, void (Server::*)(class Client &, std::vector<std::string>)> cmds;
        void    runCommand(Client &client, std::string &msg);
        void    sendServerToClient(Client &reciever, const std::string &message);
        void    sendServerToChannel(const std::string &ChannelName, const std::string &message);
        void    sendClientToChannel(Client &sender, const std::string &ChannelName, const std::string &message);

};