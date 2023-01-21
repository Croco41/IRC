#include "../Includes/server.hpp"

Server::Server(const std::string &port, const std::string &password)
	: _port(port), _password(password)
{
	_socket = launch_socket();
}

Server::~Server(void)
{
	return;
}

std::string	Server::getPassword() const
{
	return (_password);
}

int			Server::getSocket() const
{
	return (_socket);
}

int			Server::launch_socket()
{

}
