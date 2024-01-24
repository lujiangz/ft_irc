#include "../../inc/Server.hpp"

void Server::Names(class Client & client,std::vector<std::string> params)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nickname));
    if (ParamsSizeControl(client, "NAMES", params, 1, 0) != 0)
        return;
    if (IsExistChannel(params[0]))
    {
        std::string liststr = "@" +  _channels.at(params[0])->getOperator()->_nickname;
        for (std::vector<Client*>::iterator it =  _channels.at(params[0])->getMembers().begin() + 1; it !=  _channels.at(params[0])->getMembers().end(); ++it)
        {
            liststr += " " + (*it)->_nickname;
        }
        sendServerToClient(client, RPL_NAMREPLY(client._nickname, params[0], liststr));
        sendServerToClient(client, RPL_ENDOFNAMES(client._nickname, params[0]));
    }
    else
        sendServerToClient(client, RPL_ENDOFNAMES(client._nickname, params[0]));
}
