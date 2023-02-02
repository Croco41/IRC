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

//FCT MEMBRES
void	Client::writetosend(const std::string &message) const 
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
	channel->addClient(this);
	std::cout << "client : " << getNickname() << " has been added to channel : " << channel->getName() << std::endl; 
	_channel = channel; // Store a reference to the channel the client has joined

	// Get a list of nicknames of clients in the channel
	const std::vector<std::string>& nicknames = channel->getNicknames();
	// Concatenate the nicknames into a single string, separated by spaces
	std::string users;
	for (std::vector<std::string>::const_iterator it = nicknames.begin(); it != nicknames.end(); ++it) 
		users += *it + " ";
	//Send a reply to the client with the list of nicknames
	reply(RPL_NAMREPLY(_nickname, channel->getName(), users));
	//Send another reply indicating the end of the list of nicknames
	reply(RPL_ENDOFNAMES(_nickname, channel->getName()));
	//Send a message to all clients in the channel to notify them of the new client joining
	channel->sendall(RPL_JOIN(getPrefix(), channel->getName()));
	
	std::cout << _nickname << " has joined channel " << channel->getName() << std::endl; 
}
