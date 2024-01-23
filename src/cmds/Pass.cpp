#include "../../inc/Server.hpp"

void Server::Pass(Client &client, std::vector<std::string> params)
{
    if (ParamsSizeControl(client, "PASS", params, 1, 0) != 0)
        return;
    switch (client._status)
    {
    case None:
        if (PasswordMatched(params[0]))
        {
            client._status = PassReg;
            std::cout << "Pass assigned" << std::endl;
        }
        else
            sendServerToClient(client, ERR_PASSWDMISMATCH(client._nick));
        break;
    default:
        sendServerToClient(client, ERR_ALREADYREGISTERED(client._nick));
    }
}
