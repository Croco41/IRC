#ifndef COMMAND_LIST_HPP
# define COMMAND_LIST_HPP

class Command;

#include "colors.hpp"
#include "server.hpp"
#include "client.hpp"
#include "utils.hpp"

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

class PingCommand : public Command
{
	public:
		PingCommand(Server *server);
		~PingCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class PongCommand : public Command
{
	public:
		PongCommand(Server *server);
		~PongCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class JoinCommand : public Command
{
	public:
		JoinCommand(Server *server);
		~JoinCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

#endif