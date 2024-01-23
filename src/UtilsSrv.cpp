#include "../inc/Server.hpp"


int Server::ParamsSizeControl(Client& client, const std::string& Command, std::vector<std::string> params, size_t necessary, size_t optional)
{
    int err = 0;
    if (params.size() < necessary)
        err = -1;
    else if(params.size() > necessary + optional)
        err = 1;
    for (size_t i = 0; i < params.size(); i++)
    {
        if(params[i].empty())
        {
            err = -1;
            break;
        }
    }
    if (err == -1){

        if (Command == "NICK")
            sendServerToClient(client, ERR_NONICKNAMEGIVEN(client._nick));
        else
            sendServerToClient(client, ERR_NEEDMOREPARAMS(client._nick, Command));
    }
    else if (err == 1)
        sendServerToClient(client, ERR_UNKNOWNERROR(client._nick, Command, "Excessive argument is given"));
    return err;
}

bool Server::PasswordMatched(const std::string& Pass)
{
    return Pass == this->getPassword();
}

const std::string &Server::getPassword() const
{
    return _password;
}

bool Server::InvalidLetter(const std::string &Nick)
{
    if(Nick.empty())
        return false;
    std::string forbid = " .,*?!@";
    for (size_t i = 0; i < 7; i++)
    {
        if (Nick.find(forbid[i]) != std::string::npos)
            return true;
    }
    return false;
}

bool Server::IsExistClient(const std::string &ClientName)
{
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if ((*it)->_nick == ClientName)
            return true;
    }
    return false;
}

bool Server::IsExistChannel(const std::string &ChaName)
{
    std::map<std::string, class Channel*>::iterator it = _channels.find(ChaName);
    return (it != _channels.end()) ? true : false;
}

bool Server::IsInChannel(Client &client, const std::string &ChaName)
{
    return (client._channel.find(ChaName) != client._channel.end()) ? true : false;
}

bool Server::IsChannelLimitFull(const std::string &ChaName)
{
    return _channels.at(ChaName)->getMembers().size() == _channels.at(ChaName)->_userLimit;
}

bool Server::ChaKeyCheck(const std::string &ChaName)
{
    return !_channels.at(ChaName)->getKey().empty();
}

bool Server::IsOperator(Client &client, const std::string& ChaName)
{
    return _channels.at(ChaName)->getOperator()->_nick == client._nick;
}

enum Prefix Server::PrefixControl(std::string str)
{
    enum Prefix pre = PrefixClient;
    if(!str.empty() && (str[0] == '@') && str[1] == '#')
        pre = PrefixChannelOp;
    else if(!str.empty() && str[0] == '#')
        pre = PrefixChannel;
    return pre;
}