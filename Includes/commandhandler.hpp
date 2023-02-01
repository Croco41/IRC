#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

class CommandHandler;

#include "command_list.hpp"

class CommandHandler
{
	public:
		CommandHandler(Server *server);
		~CommandHandler();

		void	recup_msg(Client *client, std::string message);

	private:
		Server								*_server;
		std::map<std::string, Command *>	_commands;
};

#endif