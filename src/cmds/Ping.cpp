#include "../../inc/Server.hpp"

void Server::Ping(Client &client, std::vector<std::string> params)
{
    sendServerToClient(client, ":ircserv PONG " + params[0]); 
}
