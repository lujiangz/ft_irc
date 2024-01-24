#include "../../inc/Server.hpp"
#include <sstream>

void Server::List(class Client &client, std::vector<std::string> params)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nickname));
    if(params[0] == "")
    {
        std::ostringstream count;
        for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
        {
            count << it->second->getMembers().size();
            sendServerToClient(client, RPL_LIST(client._nickname, it->first, count.str(), it->second->_topic));
            count.clear();
        }
        return sendServerToClient(client, RPL_LISTEND(client._nickname));
    }
    //TEST ET
    if (ParamsSizeControl(client, "LIST", params, 0, 1) != 0)
        return;
    if (IsExistChannel(params[0]))
    {
        std::ostringstream count;
        count << _channels.at(params[0])->getMembers().size();
        sendServerToClient(client, RPL_LIST(client._nickname, params[0], count.str(), _channels.at(params[0])->_topic));
        sendServerToClient(client, RPL_LISTEND(client._nickname));
    }
    else
       sendServerToClient(client, RPL_LISTEND(client._nickname));
}
