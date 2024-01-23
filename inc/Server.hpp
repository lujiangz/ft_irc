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
#include "../inc/Channel.hpp"

enum Prefix
{
    PrefixClient,
    PrefixChannel,
    PrefixChannelOp,
}     

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

        // Commands 
      
        void Pass(class Client &, std::vector<std::string>);
        void Nick(class Client &, std::vector<std::string>);
        void User(class Client &, std::vector<std::string>);
        void Join(class Client &, std::vector<std::string>);
        void Mode(class Client &, std::vector<std::string>);
        void PrivMsg(class Client &, std::vector< std::string>);
    
    // void Notice(class Client &, std::vector< std::string>);
        
    // void Topic(class Client &, std::vector<std::string>);
    // void Names(class Client &, std::vector< std::string>);
    
    // void Quit(class Client &, std::vector<std::string>);
    // void Invite(class Client &, std::vector< std::string>);
    // void Kick(class Client &, std::vector<std::string>);
    
    // void Ping(class Client &, std::vector<std::string>);
    // void Part(class Client &, std::vector<std::string>);
    // void List(class Client &, std::vector<std::string>);


        bool    PasswordMatched(const std::string& Pass);
        int     ParamsSizeControl(Client& client, const std::string& Command, std::vector<std::string> params, size_t necessary, size_t optional);
        const   std::string &getPassword() const;
        bool    InvalidLetter(const std::string &Nick);
        bool    IsExistClient(const std::string &ClientName);
        bool    IsExistChannel(const std::string &ChaName);
        bool    IsInChannel(Client &client, const std::string &ChaName);
        bool    IsChannelLimitFull(const std::string &ChaName);
        bool    ChaKeyCheck(const std::string &ChaName);
        enum    Prefix PrefixControl(std::string str);
};