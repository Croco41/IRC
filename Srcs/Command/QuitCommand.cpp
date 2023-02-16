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
	std::cout << GREEN << "Clean channel dans QUIT" << RESET << std::endl;
	//vector des channels de l'user a kill
	std::vector<Channel *>	channels_userkill;
	channels_userkill = client->getChannel();
	for (std::vector<Channel *>::iterator it = channels_userkill.begin(); it != channels_userkill.end(); it++)
	{	
		client->leave_channel((*it), leave, 1);
		if ((*it)->getNbclients() == 0)
			_server->destroyChannel(*it);
	}

	//_server->~Server();
}