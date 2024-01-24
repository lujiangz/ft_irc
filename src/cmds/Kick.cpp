#include "../../inc/Server.hpp"

void Server::Kick(Client &client, std::vector<std::string> params)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nickname));
    if (ParamsSizeControl(client, "KICK", params, 2, 0) != 0)
        return;

    if(IsExistChannel(params[0]) && IsExistClient(params[1]) && IsInChannel(client, params[0]) && IsOperator(client, params[0]))
    {
        Client &kicked = findClient(params[1]);
        if(IsInChannel(kicked, params[0]))
        {
            sendServerToChannel(params[0], KICK(client._nickname, params[0], kicked._nickname));
            client._channel.at(params[0])->removeMember(kicked);
        }
        else
            sendServerToClient(client, ERR_USERNOTINCHANNEL(client._nickname, params[1], params[0]));
    }
    else
    {
        if (!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nickname, params[0]));
        else if (!IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_NOTONCHANNEL(client._nickname, params[0]));
        else
            sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._nickname, client._channel.at(params[0])->_chaName));
    }
}
