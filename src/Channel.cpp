#include "../../inc/Server.hpp"

Channel::Channel(std::string chaName, class Client* owner)
{
    _owner = &owner;
    _key = "";
    _chaName = chaName;
    _topic = "";
    _mode = ProtectedTopic;
    _userLimit = 16;
    owner._channel[chaName] = this;
}

void Channel::addMember(Client &client)
{
    client._channel.insert(std::make_pair(_chaName,this));
    _users.push_back(&client);
}

std::vector<class Client*>& Channel::getMembers() 
{
    return _users;
}

const std::string &Channel::getKey() const
{
    return _key;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
}

Client  *Channel::getOperator() const
{
    return _owner;
}

bool Channel::ChangeModeTwoParams(const std::string& ModeString)
{
    if (ModeString == "+i" || ModeString == "-i")
    {
        if (ModeString[0] == '+')
            _mode = InviteOnly//invite only
        else if (ModeString[0] == '-')
            _mode = -InviteOnly;
    }
    else if (ModeString == "+t" || ModeString == "-t")
    {
        if (ModeString[0] == '+')
            _mode = ProtectedTopic;
        else if (ModeString[0] == '-')
            _mode = -ProtectedTopic;
    }
    else if (ModeString == "-k")
        _mode = -KeyCha; 
    else
        return false;
    return true;
}


bool Channel::ChangeModeThreeParams(const std::string& ModeString, const std::string& ModeArg)
{
    if (ModeString == "+k")
    {
        if(InvalidPassword(ModeArg))
            return false;
        _key = ModeArg;
        _mode = KeyCha;
    }
    else 
        return false;
    return true;
}