#include "../../inc/Server.hpp"

bool InvalidPrefix(const std::string &Nick)
{
    if(Nick.empty())
        return false;
    std::string forbid = "$:#&/";
    for (int i = 0; i < 5; i++)
    {
        if (Nick[0] == forbid[i] || isdigit(Nick[0]))
            return true;
    }
    return false;
}

void Server::Nick(Client &client, std::vector<std::string> params)
{
    if (client._status == None)
        return sendServerToClient(client, ERR_NOTREGISTERED(client._nickname));
    if (ParamsSizeControl(client, "NICK", params, 1, 0) != 0)
        return;
    if (InvalidLetter(params[0]) || InvalidPrefix(params[0]))
        sendServerToClient(client, ERR_ERRONEUSNICKNAME(params[0]));
    else if (IsExistClient(params[0]))
        sendServerToClient(client, ERR_NICKNAMEINUSE(params[0]));
    switch (client._status)
    {
    case PassReg:
        client._nickname = ToLowercase(params[0]);
        client._username = client._nickname;
        client._realname = client._nickname;
        client._status = NickReg;
        std::cout << "Nick assigned" << std::endl;
        break;
    default:
        std::string old_nickname = client._nickname;
        client._nickname = ToLowercase(params[0]);
        std::cout << "Nick changed" << std::endl;
        sendServerToClient(client, NICK(old_nickname, client._nickname));
        for (std::map<std::string, Channel*>::iterator chan = client._channel.begin(); chan != client._channel.end(); chan++)
        {
            sendClientToChannel(client, chan->first, NICK(old_nickname, client._nickname));
        }
    }
}
