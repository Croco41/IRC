#include "../../Includes/command_list.hpp"

QuitCommand::QuitCommand(Server *server) : Command(server)
{
	return;
}

QuitCommand::~QuitCommand()
{
	return;
}

void QuitCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::string	leave;
	if (arg.empty())
		leave = "Leaving.";
	else
	{
		leave = arg.at(0);
		if (leave.at(0) == ':')
			leave = leave.substr(1);
	}
	client->reply(RPL_QUIT(client->getPrefix(), leave));
	//_server->~Server();
}