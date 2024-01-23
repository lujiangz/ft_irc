#include "../../inc/Server.hpp"

Channel::Channel(std::string chaName, Client &owner)
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
