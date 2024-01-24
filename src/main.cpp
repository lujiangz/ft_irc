#include "../inc/Server.hpp"

void signalFunction(int sig)
{
    (void)sig;
    std::cout << "Server shutdown!" << std::endl;
    exit(0);
}

bool    passCheck(std::string pass)
{
    if(pass.length() < 1 || pass.length() > 9)
        return 0;
    return 1;
}

bool    portCheck(std::string port)
{
    size_t porttemp = std::atoi(port.c_str());
    
    if(porttemp < 1024 || porttemp > 49151)
        return 0;
    return 1;
}

int main(int argc,char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    if (!portCheck(argv[1]))
    {
        std::cerr << "Port must be between 1024 and 49151" << std::endl;
        return 1;
    }
    if(!passCheck(argv[2]))
    {
        std::cerr << "Password must be between 1 and 9 characters" << std::endl;
        return 1;
    }
    signal(SIGINT, signalFunction);
    Server server(argv[1], argv[2]);
    server.setSocket();
    server.serverRun();
    return 0;

}