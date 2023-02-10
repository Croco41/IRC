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

class UserCommand : public Command
{
	public:
		UserCommand(Server *server);
		~UserCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class NickCommand : public Command
{
	public:
		NickCommand(Server *server);
		~NickCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

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

class PartCommand : public Command
{
	public:
		PartCommand(Server *server);
		~PartCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class PrivMsgCommand : public Command
{
	public:
		PrivMsgCommand(Server *server);
		~PrivMsgCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class NoticeCommand : public Command
{
	public:
		NoticeCommand(Server *server);
		~NoticeCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

#endif