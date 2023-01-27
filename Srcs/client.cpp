#include "../Includes/client.hpp"

Client::Client(const std::string &hostname, int fd, int port)
    : _hostname(hostname), _fd(fd), _port(port)
{
    return;
}

Client::~Client()
{
    return;
}

// GETTERS

std::string    Client::getHostname() const
{
    return(_hostname);
}

int            Client::getFd() const
{
    return(_fd);
}

int            Client::getPort() const
{
    return(_port);
}

std::string    Client::getRealname() const
{
    return(this->_realname);
}

std::string    Client::getUsername() const
{
    return(this->_username);
}

std::string    Client::getNickname() const
{
    return(this->_nickname);
}

std::string    Client::getPassword() const
{
    return(this->_password);
}


// SETTERS

void    Client::setRealname(const std::string &realname) 
{
    _realname = realname;
}

void    Client::setUsername(const std::string &username) 
{
    _username = username;
}

void    Client::setNickname(const std::string &nickname) 
{
    _nickname = nickname;
}

void    Client::setPassword(const std::string &password) 
{
    _password = password;
}