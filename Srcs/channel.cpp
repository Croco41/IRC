#include "../Includes/channel.hpp"

// Channel::Channel(const std::string &name, const std::string &password)
// 	: _name(name), _password(password), _admin(NULL), _clients()
// {
// 	setMaxclients(10);
// 	return;
// }

Channel::Channel(const std::string &name, const std::string &password, Client *admin)
	: _name(name), _password(password), _admin(admin), _clients()
{
	addClient(admin);
	setMaxclients(50);
	setNbclients(0);
	setModes("");
	setTopic("");
	return;
}

Channel::~Channel()
{
	return;
}

// GETTERS
std::string	Channel::getName() const
{
	return (_name);
}

std::string	Channel::getPassword() const
{
	return (_password);
}

Client* 	Channel::getAdmin()
{
	return (_admin);
}

size_t		Channel::getMaxclients() const
{
	return (_maxclients);
}

size_t		Channel::getNbclients() const
{
	return (_nbclients);
}

size_t		Channel::getNboperators() const
{
	return (_nboperators);
}

std::vector<Client *>		Channel::getOperators() const
{
	return (_operators);
}

std::string	Channel::getNicknamesList()
{
	std::string list_clients;
	
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << "valeur de it:" << (*it) << "name: " << std::endl;
		Client *client = it.operator*();
		std::string name = client->getNickname(); 
		std::cout << "name it: " << name << std::endl;
		list_clients.append(name);
	}
	return(list_clients);
}

std::vector<std::string>	Channel::getNicknames()
{ 
	std::vector<std::string>	nicknames;

	for(std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		Client *client = it.operator*();
		//on a vraiment fait un ternaire??^^
		nicknames.push_back((_admin == client ? "@" : "") + (*it)->getNickname());
		// 	// a supprimer apres debug:
		// std::cout << YELLOW << (*it)->getNickname() << RESET << std::endl;
	}
	return (nicknames);
}

std::vector<std::string>	Channel::getNicknamesOpe()
{
	std::vector<std::string>	nicknamesope;

	for(std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		Client *operators = it.operator*();
		//on a vraiment fait un ternaire??^^
		nicknamesope.push_back((_admin == operators ? "@" : "") + (*it)->getNickname());
		// 	// a supprimer apres debug:
		// std::cout << YELLOW << (*it)->getNickname() << RESET << std::endl;
	}
	return (nicknamesope);
}

std::vector<std::string>	Channel::getNicknamesClients()
{
	std::vector<std::string>	nicknamesclients;

	for(std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		Client *clients = it.operator*();
		//on a vraiment fait un ternaire??^^
		nicknamesclients.push_back((_admin == clients ? "@" : "") + (*it)->getNickname());
		// 	// a supprimer apres debug:
		// std::cout << YELLOW << (*it)->getNickname() << RESET << std::endl;
	}
	return (nicknamesclients);
}

std::string    Channel::getModes() const
{
	return(this->_modes);
}

std::string    Channel::getTopic() const
{
	return(this->_topic);
}

std::vector<Client *>	Channel::getClients() const
{
	return(this->_clients);
}

// SETTERS
void		Channel::setPassword(const std::string password)
{
	_password = password;
}

void		Channel::setMaxclients(const size_t maxclients)
{
	_maxclients = maxclients;
}

void		Channel::setNbclients(const size_t nbclients)
{
	_nbclients = nbclients;
}

void		Channel::setNboperators(const size_t nboperators)
{
	_nboperators = nboperators;
}

void    Channel::setModes(const std::string &modes) 
{
	_modes = modes;
}

void    Channel::setTopic(const std::string &topic) 
{
	_topic = topic;
}

// FCT MEMBRES
void		Channel::addClient(Client *client)
{
	_clients.push_back(client);
	setNbclients(_clients.size());
// 	// a supprimer apres debug:
// 	getNicknames();
}

void		Channel::addOperator(Client *operators)
{
	_operators.push_back(operators);
	setNboperators(_operators.size());
// 	// a supprimer apres debug:
// 	getNicknames();
}

void		Channel::removeOperator(Client *operators)
{
	std::cout << FIREBRICK << "\nCHANNEL : removeOperator - start" << RESET << std::endl;
	std::cout << "Client name = " << operators->getNickname() << std::endl;
	std::cout << "Admin name = " << _admin->getNickname() << std::endl;
	std::cout << "nb doperators = " << _operators.size() << std::endl;
	if (_operators.size() == 1)
	{
		if (!_operators.empty())
		{
			_operators.clear();
		}
		std::cout << FIREBRICK << "CHANNEL : remove Operators - end" << RESET << std::endl;
		return;
	}
	// on trouve le client dans notre vector et si on le trouve, on erase !
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it)
	{
		std::cout << "operators : " << it.operator*()->getNickname() << std::endl;
		if (*it == operators)
		{
			_operators.erase(it);
			break;
		}
	}
	setNboperators(_operators.size());
	// std::cout << "état du channel du client qu'on vient de remove : "  << client->getChannel() << RESET << std::endl;
	std::cout << FIREBRICK << "CHANNEL : removeOperators - end" << RESET << std::endl;
}

void		Channel::removeClient(Client *client)
{
	std::cout << FIREBRICK << "\nCHANNEL : removeClient - start" << RESET << std::endl;
	std::cout << "Client name = " << client->getNickname() << std::endl;
	std::cout << "Admin name = " << _admin->getNickname() << std::endl;
	std::cout << "nb de clients begin= " << _clients.size() << std::endl;
	if (client->getOperator() == true)
	{
		removeOperator(client);
		std::cout << "operators size ?" << getNboperators() << std::endl; 
	}
	if (_clients.size() == 1)
	{
		if (!_clients.empty())
		{
			_clients.clear();
			// client->setChannel(NULL);
		}
		setNbclients(_clients.size());
		std::cout << "nombre de client end1: " << _clients.size() << std::endl; 
		std::cout << FIREBRICK << "CHANNEL : removeClient - end1" << RESET << std::endl;
		return;
	}
	// si le client qui veut partir est l'admin : on set un nouvel admin
	if (_admin == client) // && _clients.size() > 1)
	{
		for (std::vector<Client*>::iterator ita = _clients.begin(); ita != _clients.end(); ++ita)
		{
			if (*ita != _admin)
			{
				_admin = *ita;
				std::cout << _admin->getNickname() << " is now admin of channel " << this->_name << std::endl;
				break;
			}
		}
	}
	// on trouve le client dans notre vector et si on le trouve, on erase !
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		std::cout << "client : " << it.operator*()->getNickname() << std::endl;
		if (*it == client)
		{
			_clients.erase(it);
			// client->setChannel(NULL);
			break;
		}
	}
	setNbclients(_clients.size());
	std::cout << "nombre de client end2: " << _clients.size() << std::endl; 
	std::cout << FIREBRICK << "CHANNEL : removeClient - end2" << RESET << std::endl;
}

bool	Channel::client_is_inchannel(Client *client)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		std::cout << "client : " << it.operator*()->getNickname() << std::endl;
		if (*it == client)
		{
			return(true);
		}
	}
	return (false);
}

bool	Channel::client_is_operator(Client *client)
{
	if(client->getOperator() == true || (_admin == client))
		return(true);
	else
	{
		return(false);
	}	
}

void		Channel::sendall(const std::string& message)
{
// Iterate over the list of clients in the channel
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		// Send the message to each client
		std::cout << GREEN;
		(*it)->reply_command(message);
		std::cout << RESET << std::endl;
	}
}

void		Channel::sendall(const std::string& message, Client *exclude)
{
	// Iterate over the list of clients in the channel
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		// Send the message to each client except one (exclude) !
		if (*it == exclude)
			continue;
		std::cout << GREEN;
		(*it)->reply_command(message);
		std::cout << RESET;
	}
}


void Channel::consolDeBUGchannel()
{
	std::cout << RED << "-----------------START CONSOL DEBUG CHANNEL-------------" << RESET << std::endl << std::endl;

//-------------------------------------------------------------------------------------------------
	std::cout << YELLOW << "-----------------CHECK CLIENTS--------------" << RESET << std::endl;

	std::string list_clients;
	
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		Client *client = it.operator*();
		std::string name = client->getNickname(); 
		list_clients.append(name);
		list_clients.append(" ");
	}
	std::cout << YELLOW << "list_clients dans le channel: " << RESET << this->getName() << std::endl;
	std::cout << GREEN << list_clients << RESET << std::endl;
	std::cout << YELLOW << "size list_clients: " << RESET << std::endl;
	std::cout << GREEN << _clients.size() << RESET << std::endl;

//-------------------------------------------------------------------------------------------------
	std::cout << YELLOW << "-----------------CHECK OPERATORS--------------" << RESET << std::endl;

	std::string list_operators;
	
	for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		Client *client = it.operator*();
		std::string name = client->getNickname(); 
		list_operators.append(name);
		list_operators.append(" ");
	}
	std::cout << YELLOW << "list_operators dans le channel: " << RESET << this->getName()  << std::endl;
	std::cout << GREEN << list_operators << RESET << std::endl;
	std::cout << YELLOW << "size list_operators sans admin: " << RESET << std::endl;
	std::cout << GREEN << _operators.size() << RESET << std::endl;
	// 	std::cout << YELLOW << "size list_operators avec admin: " << RESET << std::endl;
	// std::cout << GREEN << getNboperators() << RESET << std::endl;

//-------------------------------------------------------------------------------------------------
	std::cout << YELLOW << "-----------------CHECK OTHER--------------" << RESET << std::endl;

	std::cout << YELLOW << "liste des modes dans le channel: " << RESET << this->getName() << std::endl;
	std::cout << GREEN <<_modes << RESET << std::endl;
	std::cout << YELLOW << "a completer " << RESET << std::endl;

std::cout << RED << "-----------------END CONSOL DEBUG CHANNEL-------------" << RESET << std::endl << std::endl;
}