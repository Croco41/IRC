#include "../Includes/client.hpp"

Client::Client(const std::string &hostname, int fd, int port)
	: _hostname(hostname), _fd(fd), _port(port)
{
	_realname = "parsing realname";
	_username = "parsing username";
	_nickname = "parsing nickname";
	return;
}

Client::~Client()
{
	return;
}

// GETTERS

std::string	Client::getHostname() const
{
	return(_hostname);
}

int			Client::getFd() const
{
	return(_fd);
}

int			Client::getPort() const
{
	return(_port);
}

std::string	Client::getRealname() const
{
	return(_realname);
}

std::string	Client::getUsername() const
{
	return(_username);
}

std::string	Client::getNickname() const
{
	return(_nickname);
}


// SETTERS

void		Client::setRealname(const std::string &realname) 
{
	_realname = realname;
}

void		Client::setUsername(const std::string &username) 
{
	_username = username;
}

void		Client::setNickname(const std::string &nickname) 
{
	_nickname = nickname;
}