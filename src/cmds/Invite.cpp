#include "../../inc/Server.hpp"

void Server::Invite(Client &client, std::vector<std::string> params)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nickname));
    if (ParamsSizeControl(client, "INVITE", params, 2, 0) != 0)
        return;
    if (IsExistClient(params[1]) && IsExistChannel(params[0]) && IsInChannel(client,params[0]))
    {
        Client& invited = findClient(params[1]);
        invited._invChan = params[0];
        if (IsInChannel(invited, params[0]))
            return sendServerToClient(client, ERR_USERONCHANNEL(client._nickname, invited._nickname, params[0]));
            
        std::vector<std::string> channel;
        channel.push_back(params[0]);
        if ((_channels.at(params[0])->_mode & InviteOnly) && IsOperator(client, params[0]))  
            Join(invited, channel);
        else if (!(_channels.at(params[0])->_mode & InviteOnly))
        {
            sendServerToClient(client, RPL_INVITING(client._nickname,invited._nickname,params[0]));
            sendServerToClient(invited, INVITE(client._nickname,invited._nickname,params[0]));
            Join(invited, channel);
        }
        else
            sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._nickname, params[0]));
    }
    else
    {
        if(!IsExistClient(params[1]))
            sendServerToClient(client, ERR_NOSUCHNICK(client._nickname, params[1]));
        else if(!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nickname, params[0]));
        else if(!IsInChannel(client,params[0]))
            sendServerToClient(client,  ERR_NOTONCHANNEL(client._nickname, params[0]));
    }
}
