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
	setMaxclients(10);
	setNbclients(0);
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

std::vector<std::string>	Channel::getNicknames()
{
	std::vector<std::string>	nicknames;

	for(std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		Client *client = it.operator*();
		nicknames.push_back((_admin == client ? "@" : "") + (*it)->getNickname());
	}
	return (nicknames);
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

// FCT MEMBRES
void		Channel::addClient(Client *client)
{
	_clients.push_back(client);
	setNbclients(_clients.size());
}

void		Channel::removeClient(Client *client)
{
	std::cout << FIREBRICK << "\nCHANNEL : removeClient - start" << RESET << std::endl;
	std::cout << "Client name = " << client->getNickname() << std::endl;
	std::cout << "Admin name = " << _admin->getNickname() << std::endl;
	std::cout << "nb de clients = " << _clients.size() << std::endl;
	if (_clients.size() == 1)
	{
		if (!_clients.empty())
		{
			_clients.clear();
			// client->setChannel(NULL);
		}
		std::cout << FIREBRICK << "CHANNEL : removeClient - end" << RESET << std::endl;
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
	// std::cout << "état du channel du client qu'on vient de remove : "  << client->getChannel() << RESET << std::endl;
	std::cout << FIREBRICK << "CHANNEL : removeClient - end" << RESET << std::endl;
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
