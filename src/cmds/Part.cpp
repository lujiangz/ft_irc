#include "../../inc/Server.hpp"

void Server::Part(Client &client, std::vector<std::string> params)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nickname));
    if (ParamsSizeControl(client, "PART", params, 1, 1) != 0)
        return;

    if (IsExistChannel(params[0]) && IsInChannel(client, params[0]))
    {
        if (_channels.at(params[0])->getMembers().size() == 1)
        {
            sendServerToClient(client, PART(client._nickname, params[0] + " :closed the channel"));
            _channels.at(params[0])->removeMember(client);
            Channel* chan = _channels.at(params[0]);
            _channels.erase(params[0]);
            delete chan;
        }
        else if (IsOperator(client, params[0]) && _channels.at(params[0])->getMembers().size() > 1)
        {
            sendServerToChannel(params[0], PART(client._nickname, params[0]));
            _channels.at(params[0])->removeMember(client);
            Client* next_op = _channels.at(params[0])->getMembers().front();
            _channels[params[0]]->setOperator(next_op);
            sendServerToChannel(params[0], MODE(std::string("ircserv"), params[0], "+o", next_op->_nickname));
        }
        else
        {
            sendServerToChannel(params[0], PART(client._nickname, params[0]));
            _channels.at(params[0])->removeMember(client);
        }
    }
    else
    {
        if (!IsExistChannel(params[0]))
            sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nickname, params[0]));
        else if (!IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_NOTONCHANNEL(client._nickname, params[0]));
    }
}
