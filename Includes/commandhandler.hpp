#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

class CommandHandler;

#include "command_list.hpp"

class CommandHandler
{
	public:
		CommandHandler(Server *server);
		~CommandHandler();

	private:
		Server		*_server;
		std::map<std::string, Command *>	_commands;
};

#endif