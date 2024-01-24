#include "../../inc/Server.hpp"

void Server::Join(Client &client,std::vector<std::string> params)
{

    if(client._status != UserReg)
        return sendServerToClient(client,ERR_NOTREGISTERED(client._nickname));
    if (ParamsSizeControl(client, "JOIN", params, 1, 1) != 0)
        return;

    size_t found = params[0].find('\x07');  // ^G's ASCII value 0x07
    if (found != std::string::npos) {
        std::cout << "Found ^G" << std::endl;

    if (InvalidLetter(params[0]) || params[0][0] != '#')
        sendServerToClient(client, ERR_UNKNOWNERROR(client._nickname, "JOIN", "Forbidden letter in use as Channel name or didn't use #."));

    if (IsExistChannel(params[0]))
    {
        if (IsInChannel(client, params[0]))
            sendServerToClient(client, ERR_USERONCHANNEL(client._nickname, params[1], params[0]));
        else if (IsChannelLimitFull(params[0]))
            sendServerToClient(client,ERR_CHANNELISFULL(client._nickname, params[0]));
        else if (_channels.at(params[0])->_mode & InviteOnly && client._invChan != params[0])
            sendServerToClient(client,ERR_INVITEONLYCHAN(client._nickname, params[0]));
        else if (params.size() < 2 && ChaKeyCheck(params[0]))//join kanaladı parola
            sendServerToClient(client,ERR_BADCHANNELKEY(client._nickname, params[0]));
        else if (params.size() == 2 && (_channels.at(params[0])->getKey() == params[1]))
            sendServerToClient(client,ERR_BADCHANNELKEY(client._nickname, params[0]));
        else
        {
            client._invChan = "";
            _channels.at(params[0])->addMember(client);
            sendServerToChannel(params[0], JOIN(client._nickname, params[0]));
            Topic(client, std::vector<std::string>(1,params[0]));
            Names(client, std::vector<std::string>(1,params[0]));
        }
    }       
    else
    {
       
        if (client._channel.size() == 4)
            sendServerToClient(client,ERR_TOOMANYCHANNELS(client._nickname, params[0]));
        else
        {
            Channel* newChannel = new Channel(params[0], client);
            _channels.insert(std::make_pair(std::string(params[0]), newChannel));
            sendServerToClient(client, JOIN(client._nickname, params[0]));
            newChannel->addMember(client);
           if(params.size() == 2)
           {
                std::vector<std::string> vec;
                vec.push_back(params[0]);
                vec.push_back("+k");
                vec.push_back(params[1]);
                Mode(client,vec);
            }       
            sendServerToClient(client, MODE(std::string("ircserv"), params[0], "+o", client._nickname));
            Topic(client, std::vector<std::string>(1,params[0]));
            Names(client, std::vector<std::string>(1,params[0]));
        }
    }
}
}
