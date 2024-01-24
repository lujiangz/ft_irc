#include "../../inc/Server.hpp"

const std::map<char, int> ModeMap()
{
    static std::map<char, int> modes;
    modes['i'] = InviteOnly;
    modes['k'] = KeyCha;
    modes['t'] = ProtectedTopic;
    modes['l'] = ChaLimit;
    return modes;
}

void Server::Mode(Client &client, std::vector<std::string> params)
{
    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nick));
    if (ParamsSizeControl(client, "MODE", params, 1, 2) != 0)
        return;      
    size_t count = params.size();
    const std::map<char, int>& modes = ModeMap();
    if (IsExistChannel(params[0]))
    {
        std::string modestr = "+";
        for (std::map<char, int>::const_iterator it = modes.begin(); it != modes.end(); it++)
        {
            if (_channels.at(params[0])->_mode & it->second)
                modestr += it->first;
        }
        if (count == 1)//test et
            sendServerToClient(client, RPL_CHANNELMODEIS(client._nick, params[0], modestr));
        else if (IsOperator(client, params[0]))
        {
            if(count == 2)
            {
                if (_channels.at(params[0])->ChangeModeTwoParams(params[1]))
                   sendServerToChannel(params[0], MODE(client._nick, params[0], params[1], "")); 
                else
                    sendServerToClient(client, ERR_UNKNOWNMODE(client._nick, params[1]));
            }
            else if(count == 3)
            {
                if (_channels.at(params[0])->ChangeModeThreeParams(params[1], params[2]))
                    sendServerToChannel(params[0], MODE(client._nick, params[0], params[1], params[2])); 
                else
                    sendServerToClient(client, ERR_UNKNOWNMODE(client._nick, params[1]));
             }
        }
        else
            sendServerToClient(client, ERR_CHANOPRIVSNEEDED(client._nick, client._channel.at(params[0])->_name));
    }
    else
        sendServerToClient(client, ERR_NOSUCHCHANNEL(client._nick, params[0]));
}
