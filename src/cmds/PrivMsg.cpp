#include "../../inc/Server.hpp"

void Server::PrivMsg(class Client &client, std::vector<std::string > params)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nickname));
    size_t count = params.size();
    if(count == 1)
        return sendServerToClient(client,ERR_NOTEXTTOSEND(client._nickname));
    else if (count == 0)
        return sendServerToClient(client, ERR_NORECIPIENT(client._nickname, "PRIVMSG"));
    enum Prefix pre = PrefixControl(params[0]);
    std::string message = (params[1][0] == ':') ? params[1].substr(1) : params[1];
    for (size_t i = 2; i < count; i++)
            message += " " + params[i];
    switch (pre)
    {
    case PrefixClient:
        if(IsExistClient(params[0]))
        {
            Client& toClient = findClient(params[0]);
            sendServerToClient(toClient, PRIVMSG(client._nickname, toClient._nickname, message));
        }
        else
            sendServerToClient(client,ERR_NOSUCHNICK(client._nickname, params[0]));
        break;
    case PrefixChannelOp:
        if (IsExistChannel(params[1].substr(1)))
        {
            Client& op = *_channels.at(params[1].substr(1))->getOperator();
            sendServerToClient(op, PRIVMSG(client._nickname, op._nickname, message));
        }
        else
            sendServerToClient(client,ERR_NOSUCHCHANNEL(client._nickname,params[1].substr(1)));
        break;
    case PrefixChannel:
        if(IsExistChannel(params[0]) && IsInChannel(client,params[0]))
            sendClientToChannel(client,params[0], PRIVMSG(client._nickname, params[0], message));
        else if(!IsInChannel(client,params[0]))
            sendServerToClient(client,ERR_CANNOTSENDTOCHAN(client._nickname,params[0]));
        else
            sendServerToClient(client,ERR_NOSUCHCHANNEL(client._nickname, params[0]));
        break;
    default:
        break;
    }
}
