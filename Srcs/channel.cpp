#include "../Includes/channel.hpp"

Channel::Channel(const std::string &name, const std::string &password)
	: _name(name), _password(password)
{
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

// SETTERS
void		Channel::setPassword(const std::string password)
{
	_password = password;
}

void		Channel::setMaxclients(const size_t maxclients)
{
	_maxclients = maxclients;
}
