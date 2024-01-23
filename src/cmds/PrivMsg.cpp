#include "../../inc/Server.hpp"

void Server::PrivMsg(class Client &client, std::vector<std::string > params)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    size_t count = params.size();
    if(count == 1)
        return sendServerToClient(client,ERR_NOTEXTTOSEND(client._nick));
    else if (count == 0)
        return sendServerToClient(client, ERR_NORECIPIENT(client._nick, "PRIVMSG"));
    //burda kaldın
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
            sendServerToClient(toClient, PRIVMSG(client._nick, toClient._nick, message));
        }
        else
            sendServerToClient(client,ERR_NOSUCHNICK(client._nick, params[0]));
        break;
    case PrefixChannelOp:
        if (IsExistChannel(params[1].substr(1)) && !IsBannedClient(client,params[0]))
        {
            Client& op = *_channels.at(params[1].substr(1))->getOperator();
            sendServerToClient(op, PRIVMSG(client._nick, op._nick, message));
        }
        else if(IsBannedClient(client,params[1].substr(1)))
            sendServerToClient(client,ERR_CANNOTSENDTOCHAN(client._nick,params[1].substr(1)));
        else
            sendServerToClient(client,ERR_NOSUCHCHANNEL(client._nick,params[1].substr(1)));
        break;
    case PrefixChannel:
        if(IsExistChannel(params[0]) && IsInChannel(client,params[0]) && !IsBannedClient(client,params[0]))
            sendClientToChannel(client,params[0], PRIVMSG(client._nick, params[0], message));
        else if(!IsInChannel(client,params[0]) || IsBannedClient(client,params[0]))
            sendServerToClient(client,ERR_CANNOTSENDTOCHAN(client._nick,params[0]));
        else
            sendServerToClient(client,ERR_NOSUCHCHANNEL(client._nick, params[0]));
        break;
    default:
        break;
    }
}
