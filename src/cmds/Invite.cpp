#include "../../inc/Server.hpp"

void Server::Invite(Client &client, std::vector<std::string> params)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nickname));
    if (ParamsSizeControl(client, "INVITE", params, 2, 0) != 0)
        return;
    if (IsExistClient(params[1]) && IsExistChannel(params[0]) && IsInChannel(client,params[0]) && IsOperator(client, params[0]))
    {
            Client& invited = findClient(params[1]);
            if (IsInChannel(invited, params[0]))
                return sendServerToClient(client, ERR_USERONCHANNEL(client._nickname, invited._nickname, params[0]));
            invited._invChan = params[0];
             sendServerToClient(client, RPL_INVITING(client._nickname,invited._nickname,params[0]));
             sendServerToClient(invited, INVITE(client._nickname,invited._nickname,params[0]));      
    }
    else
    {
        if(!IsExistClient(params[1]))
            sendServerToClient(client, ERR_NOSUCHNICK(client._nickname, params[1]));
        else if(!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nickname, params[0]));
        else if(!IsInChannel(client,params[0]))
            sendServerToClient(client,  ERR_NOTONCHANNEL(client._nickname, params[0]));
        else
            sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._nickname, params[0]));
    }
}
