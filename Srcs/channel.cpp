#include "../Includes/channel.hpp"

Channel::Channel(const std::string &name, const std::string &password)
	: _name(name), _password(password)
{
	setMaxclients(10);
	return;
}

Channel::Channel(const std::string &name, const std::string &password, Client *admin)
	: _name(name), _password(password), _admin(admin)
{
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
}

void		Channel::sendall(const std::string& message)
{
  // Iterate over the list of clients in the channel
  for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
  {
    // Send the message to each client
	std::cout << GREEN;
    (*it)->reply(message);
	std::cout << RESET;
  }
} 
