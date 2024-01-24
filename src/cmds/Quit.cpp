#include "../../inc/Server.hpp"

void Server::Quit(Client &client, std::vector<std::string>)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nickname));
        
    // apart from channels
    std::vector<std::string> chans;
    for (std::map<std::string, Channel*>::iterator it = client._channel.begin(); it != client._channel.end(); it++)
        chans.push_back(it->first);
    for (std::vector<std::string>::iterator it = chans.begin(); it != chans.end(); it++)
    {
        std::vector<std::string> par;
        par.push_back(*it);
        Part(client, par);
    }
    // make client offline
    client._online = false;
}