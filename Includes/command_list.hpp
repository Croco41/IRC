#ifndef COMMAND_LIST_HPP
# define COMMAND_LIST_HPP

class Command;

#include "server.hpp"
#include "client.hpp"

// Command est ici une classe interface qui donne le patron de chaque commande

class Command
{
	public:
		Command(Server *server) : _server(server){};
		virtual ~Command(){};

		virtual void execute(Client *client, std::vector<std::string> arg) = 0;

	protected:
		Server	*_server;
};

// ici on aura toutes les classes dérivées de Command : join, quit, user, nick...

class JoinCommand : public Command
{
	public:
		JoinCommand(Server *server);
		~JoinCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

#endif