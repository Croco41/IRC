#include "../Includes/client.hpp"

Client::Client(const std::string &hostname, int fd, int port)
	: _hostname(hostname), _fd(fd), _port(port), _realname(""), _username(""), _nickname(""), _password(""), _modes(""), _isoperator(0), _isregistered(0), _Cchannels()  
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
std::string		Client::getHostname() const
{
	return(_hostname);
}

int				Client::getFd() const
{
	return(_fd);
}

int				Client::getPort() const
{
	return(_port);
}

std::string		Client::getRealname() const
{
	return(this->_realname);
}

std::string		Client::getUsername() const
{
	return(this->_username);
}

std::string		Client::getNickname() const
{
	return(this->_nickname);
}

std::string		Client::getPassword() const
{
	return(this->_password);
}

std::string		Client::getModes() const
{
	return(this->_modes);
}

bool			Client::getOperator() const
{
	return(this->_isoperator);
}

bool			Client::getRegistered() const
{
	return(this->_isregistered);
}

std::string		Client::getListChannel()
{
	std::string list_channel;
	for (std::vector<Channel *>::iterator it = _Cchannels.begin(); it != _Cchannels.end(); it++)
	{
		// std::cout << "valeur de it:" << (*it) << "name: " << std::endl;
		Channel *channel = it.operator*();
		std::string name = channel->getName(); 
		// std::cout << "name it: " << name << std::endl;
		list_channel.append(name);
	}
	return(list_channel);
}

std::vector<Channel *>	Client::getChannel() const
{
	return(_Cchannels);
}

bool			Client::findChannel(std::string chan_name)
{
	std::cout << RED << "CLIENT : findChannel - start" << RESET << std::endl;
	for (std::vector<Channel *>::iterator it = _Cchannels.begin(); it != _Cchannels.end(); it++)
	{
		std::cout << "je suis dans le for et cchannel size" << _Cchannels.size() << std::endl;
		std::cout << "on entre dans le for ?" << chan_name << std::endl;
		// if (_Cchannels.size() == )
		if (it.operator*()->getName() == chan_name)
			return(true);
	}
	return (false);
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

void    Client::setModes(const std::string &modes) 
{
	_modes = modes;
}

void    Client::setOperator(const bool &isoperator) 
{
	_isoperator = isoperator;
	std::vector<Channel *>::iterator	it;
	std::string nickname = getNickname();
	for (it = _Cchannels.begin(); it != _Cchannels.end(); it++)
	{
		Channel *channel = it.operator*();
		std::vector<std::string> nickope = channel->getNicknamesOpe();
		if (isoperator == false)
		{
			if(std::find(nickope.begin(), nickope.end(), nickname) != nickope.end())
			{
					channel->removeOperator(this);
					continue;
					//si on change un droit, faut l'enlever dans les operateurs des chans
					//verifier si à la création d'un chan on check les droits du creatoeurs ou pendant join
			}
		}
		if (isoperator == true)
		{
			if(std::find(nickope.begin(), nickope.end(), nickname) != nickope.end())
			{
					channel->addOperator(this);
					continue;
			}
		}
	}
}

void    Client::setRegistered(const bool &isregistered) 
{
	_isregistered = isregistered;
}

// void	Client::setChannel(Channel *channel)
// {
// 	_channel = channel;
// }

void	Client::setChannel(Channel *channel)
{
	std::cout << "je suis dans mon set channel de mon client: " << _nickname << " et le chan ajouté: " << channel->getName() << std::endl; 
	_Cchannels.push_back(channel);
}

//FCT MEMBRES
void	Client::writetosend(const std::string &message) const 
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
	//writetosend(":" + reply);
	//std::cout << ":" << getPrefix() << " " << reply << std::endl;
}

void	Client::reply_command(const std::string &reply)
{
	//writetosend(":" + getPrefix() + " " + reply);
	writetosend(reply);
	//std::cout << ":" << getPrefix() << " " << reply << std::endl;
}

void	Client::join_channel(Channel *channel)
{
	std::cout << GREEN << "\nCLIENT : join_channel - start " << channel->getName() << RESET << std::endl;
	if (channel->getNbclients() != 0)
	{
		channel->addClient(this);
		if (channel->client_is_operator(this) == true)
		{
			channel->addOperator(this);
			std::vector<Client *> _operatorschan = channel->getOperators();
			size_t sizeop = _operatorschan.size();
			channel->setNboperators(sizeop);
		}
		setChannel(channel);
	}
	else
		channel->setNbclients(1);
	
	std::cout << "client : " << getNickname() << " has been added to channel : " << channel->getName() << std::endl; 
//	_channel = channel; // Store a reference to the channel the client has joined
	//setChannel(channel);
	// Get a list of nicknames of clients in the channel
	const std::vector<std::string>& nicknames = channel->getNicknamesClients();
	// Concatenate the nicknames into a single string, separated by spaces
	std::string users;
	for (std::vector<std::string>::const_iterator it = nicknames.begin(); it != nicknames.end(); ++it) 
		users += *it + " ";
	//Send a reply to the client with the list of nicknames
	reply(RPL_NAMREPLY(_nickname, channel->getName(), users));
	//Send another reply indicating the end of the list of nicknames
	reply(RPL_ENDOFNAMES(_nickname, channel->getName()));
	//Send a message to all clients in the channel to notify them of the current client joining
	channel->sendall(RPL_JOIN(getPrefix(), channel->getName()));
	if(channel->getTopic() == "")
	{
		reply(RPL_NOTOPIC(getNickname(), channel->getName()));
	}
	else
	{
		reply(RPL_TOPIC(getNickname(), channel->getName(), channel->getTopic()));
	}	
	std::cout << _nickname << " has joined channel " << channel->getName() << std::endl;
	std::cout << YELLOW << _nickname << "estdans les channel suivants: " << getListChannel() << RESET << std::endl;
	std::cout << GREEN << "CLIENT : join_channel - end " << channel->getName() << RESET << std::endl;
}

void	Client::leave_channel(Channel *channel, std::string message, bool kill)
{
	std::cout << ORANGE << "\nCLIENT : leave_channel - start" << RESET << std::endl;
	if (!channel)
		return;
	std::cout << ORANGE << "leave_channel du Client !" << RESET << std::endl;
	std::cout << PURPLE << "Send reply message is: " << message << std::endl;
	std::cout << GREEN << "Rpl sent to socket: " << _fd << std::endl;
	if (kill == 0)
		channel->sendall(RPL_PART(getPrefix(), channel->getName(), message));
	channel->removeClient(this);

	std::vector<Channel *>::iterator	it;
	for (it = _Cchannels.begin(); it != _Cchannels.end(); it++)
	{	
		//std::cout << ORANGE << "nom du channel: " << it.operator*()->getName() << RESET << std::endl;
		if (*it == channel)
		//if (it.operator*()->getName() == channel->getName())
		{
			std::cout << ROYALBLUE << "taille du vector de channels : " << _Cchannels.size() << RESET << std::endl;
		
			// it.operator*()->sendall(RPL_PART(getPrefix(), chan_name, message));
			// it.operator*()->removeClient(this);
			_Cchannels.erase(it);
			std::cout << ROYALBLUE << "taille du vector de channels apres erase : " << _Cchannels.size() << RESET << std::endl;

			// if (_Cchannels.size() == 0);
			// 	_Cchannels.push_back("");
		break;
		}
	}
	std::cout << ORANGE << "CLIENT : leave_channel - end" << RESET << std::endl;
}
