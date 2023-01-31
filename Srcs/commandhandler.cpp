#include "../Includes/commandhandler.hpp"

CommandHandler::CommandHandler(Server *server)
	: _server(server)
{
	_commands["JOIN"] = new JoinCommand(_server);
}

CommandHandler::~CommandHandler()
{
	for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
		delete it->second;
}
