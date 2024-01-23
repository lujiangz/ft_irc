#include "../inc/Server.hpp"

void Server::CmdMap()
{
    cmds["PASS"] = &Server::Pass;
    cmds["NICK"] = &Server::Nick;
    cmds["USER"] = &Server::User;
    cmds["PING"] = &Server::Ping;
    cmds["QUIT"] = &Server::Quit;
    cmds["JOIN"] = &Server::Join;
    cmds["PART"] = &Server::Part;
    cmds["TOPIC"] = &Server::Topic;
    cmds["NAMES"] = &Server::Names;
    cmds["INVITE"] = &Server::Invite;
    cmds["MODE"] = &Server::Mode;
    cmds["KICK"] = &Server::Kick;
    cmds["NOTICE"] = &Server::Notice;
    cmds["PRIVMSG"] = &Server::PrivMsg;
    cmds["LIST"] = &Server::List;
    // buraya bakacaz
}

Server::Server(const std::string &Port, const std::string &Password) : 
{
    CmdMap();
    this->_port = std::atoi(Port.c_str());
    this->_password = Password;
    this->_channels = std::map<std::string, class Channel*>();
    this->_clients =  std::vector<class Client*>();
}

void    Server::setSocket()
{
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd == -1)
    {
        std::cerr << "Error: socket creation failed" << std::endl;
        exit(1);
    }

    int opt = 1;
    if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cerr << "Error: setsockopt failed" << std::endl;
        close(_socketFd);
        exit(1);
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_socketFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Error: bind failed" << std::endl;
        close(_socketFd);
        exit(1);
    }
    _serverAddr = serverAddr;
    // max client 10
    if (listen(_socketFd, 10) == -1)
    {
        std::cerr << "Error: listen failed" << std::endl;
        close(_socketFd);
        exit(1);
    }
    std::cout << "Server is running on port " << _port << std::endl;
}

void    Server::serverRun()
{
    while(true)
    {
        fd_set readfds;
        FD_ZERO(&readfds);

        int maxSocket = _socketFd;
        FD_SET(_socketFd, &readfds);

        // max socket set for select

        for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
        {
            int socketFd = (*it)->getSocketFd();
            FD_SET(socketFd, &readfds);
            if (socketFd > maxSocket)
                maxSocket = socketFd;
        }

        if (select(maxSocket + 1, &readfds, NULL, NULL, NULL) == -1)
        {
            std::cerr << "Error: select failed" << std::endl;
            continue;            
        }

        // check if there is a new connection

        if (FD_ISSET(_socketFd, &readfds))
        {
            sockaddr_in clientAddr;
            socklen_t clientAddrSize = sizeof(clientAddr);
            int clientSocket = accept(_socketFd, (sockaddr*)&clientAddr, &clientAddrSize);
            if (clientSocket == -1)
            {
                std::cerr << "Error: accept failed" << std::endl;
                continue;
            }
            std::cout << "New client connected" << clientSocket <<std::endl;
            
            fcntl(clientSocket, F_SETFL, O_NONBLOCK);

            Client *newClient = new Client(clientSocket);
            newClient->checkHostname(_serverAddr);
            _clients.push_back(newClient);
        }
        readData(readfds);
    }
}

void    Server::readData(fd_set readfds)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        int socketFd = (*it)->getSocketFd();
        if (FD_ISSET(socketFd, &readfds))
        {
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            int readSize = recv(socketFd, buffer, 1023, 0);
            if (readSize == 0)
            {
                std::cout << "Client disconnected" << socketFd << std::endl;
                (*it)->_online = false;
                // Quit
                continue;
            }
            if (readSize == -1)
            {
                std::cerr << "Error: recv failed" << std::endl;
                                // Quit

                continue;
            }
            buffer[readSize] = '\0';
            std::string msg(buffer);
            std::cout << "Received: " << msg << std::endl;
            std::string cmd = msg.substr(0, msg.find(" "));
            std::string params = msg.substr(msg.find(" ") + 1);
           
           runCommand(*it,msg);// buraya bakacaz hugg
        }
        ++it;
    }
}

void    Server::runCommand(Client &client, std::string &msg)
{
    std::vector<std::string> str = split(msg,"\r\n");
    for(std::vector<std::string>::iterator it = str.begin(); it != str.end()-1; ++it)
    {
        std::cout <<"line: " << *it << std::endl;
        size_t spacePos = (it->find(' ')  != std::string::npos) ? it->find(' ') : it->size();
        std::string command = it->substr(0, spacePos);
        std::cout <<"cmd: " << command << std::endl;
        spacePos = (spacePos == it->size()) ? spacePos -1 : spacePos;
        if (cmds.find(command) != cmds.end())
            (this->*cmds.at(command))(*client, split(it->substr(spacePos + 1), " "));
    }
}

void Server::sendServerToClient(Client &reciever, const std::string &message)
{
    std::string formattedMessage = message + "\r\n";
    if (send(reciever.getSocketFd(), formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
    {
        std::cerr << "Failed to send chat message between "
                  << "ircserv"
                  << " -> " << reciever._nickname << std::endl;
    }
}
// channel ekle la buraya bakacaz
void Server::sendServerToChannel(const std::string &ChannelName, const std::string &message)
{
    std::string formattedMessage = message + "\r\n";
    std::vector<Client*>::iterator client = _channels.at(ChannelName)->getMembers().begin();
    std::vector<Client*>::iterator end = _channels.at(ChannelName)->getMembers().end();
    for (; client != end; client++)
    {
        if (send((*client)->getSocketFd(), formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
        {
            std::cerr << "Failed to send chat message between " << ChannelName << " -> " << (*client)->_nick << std::endl;
            continue;
        }
    }
}
// channel ekle la buraya bakacaz

void Server::sendClientToChannel(Client &sender, const std::string &ChannelName, const std::string &message)
{
    if (sender._channel.empty())
        return ;
    std::string formattedMessage = message + "\r\n";
    std::vector<Client*>::iterator client = _channels.at(ChannelName)->getMembers().begin();
    std::vector<Client*>::iterator end = _channels.at(ChannelName)->getMembers().end();
    for (; client != end; client++)
    {
        if ((*client)->getSocketFd() != sender.getSocketFd())
        {
            if (send((*client)->getSocketFd(), formattedMessage.c_str(), formattedMessage.length(), 0) == -1)
            {
                std::cerr << "Failed to send chat message between " << sender._nickname << " -> " << (*client)->_nickname << std::endl;
                continue;
            }
        }
    }
}