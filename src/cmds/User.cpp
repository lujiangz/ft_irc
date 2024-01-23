#include "../../inc/Server.hpp"

void Server::User(Client &client, std::vector<std::string>params)
{
    if (client._status < NickReg)
        return sendServerToClient(client, ERR_NOTREGISTERED(client._nick));
    else if (client._status == UserReg)
        return sendServerToClient(client, ERR_ALREADYREGISTERED(client._nick));
   
    if (ParamsSizeControl(client, "USER", params, 3, 2) != 0)  
        return;
    size_t count = params.size();
   
    client._username = params[0];
    if(count > 3)
    {
        if(params[3][0] == ':')
            client._realname = params[3].substr(1);
        else
            client._realname = params[3];
        for (size_t i = 4; i < count; i++)
            client._realname += " " + params[i];
    }
    client._status = UserReg;
    std::cout << "Username assigned" << std::endl;
}