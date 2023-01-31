#include "../Includes/client.hpp"

Client::Client(const std::string &hostname, int fd, int port)
    : _hostname(hostname), _fd(fd), _port(port)
{
	if (_hostname.size() > 63)
	{
		std::stringstream ss;
		ss << port;
		_hostname = ss.str();
	}
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

Channel*		Client::getChannel() const
{
	return(_channel);
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

void	Client::setChannel(Channel *channel)
{
	_channel = channel;
}

void Client::writetosend(const std::string &message) const 
{
	std::cout << "here? in write to send?" << std::endl;
	std::cout << "---> " << message << std::endl;

	std::string buffer = message + "\r\n";
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while sending message to client.");
}

std::string Client::getPrefix() const 
{
	return (_nickname + "!" + _username + "@" + _hostname);
}

void	Client::reply(const std::string &reply)
{
	writetosend(":" + getPrefix() + " " + reply);
	//std::cout << ":" << getPrefix() << " " << reply << std::endl;
}

void	Client::join_channel(Channel *channel)
{
	std::cout << "Il va falloir coder la fonction qui permet au client de join le channel " << channel->getName() << std::endl;
}
