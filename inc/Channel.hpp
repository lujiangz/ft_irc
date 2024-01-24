#pragma once

#include <iostream>
#include <vector>
#include <map>

enum Mode
{
    ProtectedTopic = 1,
    InviteOnly = 2,
    KeyCha = 4,
    ChaLimit = 8
};

class Channel {
    private:
        class Client* _owner;
        std::string _key;
        std::vector<class Client*> _users;
    public:
        std::string _chaName;
        std::string _topic;
        int _mode;
        unsigned int _userLimit;

        Channel(std::string chaName, class Client& owner);
        void addMember(Client &client);
        std::vector<class Client*>& getMembers();
        bool    ChangeModeTwoParams(const std::string& ModeString);
        bool    ChangeModeThreeParams(const std::string& ModeString, const std::string& ModeArg);
        Client  *getOperator() const;
        void    removeMember(Client &client);
        void    setOperator(Client *client);
        void    setKey(const std::string &key);
        const std::string &getKey() const;


};