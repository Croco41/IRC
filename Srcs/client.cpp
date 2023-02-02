#include "../Includes/client.hpp"

//hello ISA coucou !

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

void Client::writetosend(const std::string &message) const 
{
	std::cout << "---> " << message << std::endl;

	std::string buffer = message + "\r\n";
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while sending message to client.");
}

std::string Client::getPrefix() const 
{
	//return (_nickname + "@" + _hostname);
	//return (_username + "@" + _hostname);
	return (_nickname + "!" + _username + "@" + _hostname);
}

void	Client::reply(const std::string &reply)
{
	writetosend(":" + getPrefix() + " " + reply);
	//std::cout << ":" << getPrefix() << " " << reply << std::endl;
}

