#include "command_list.hpp"

PongCommand::PongCommand(Server *server) : Command(server)
{
	return;
}

PongCommand::~PongCommand()
{
	return;
}

void PongCommand::execute(Client *client, std::vector<std::string> arg)
{
	//std::cout << FUCHSIA << "\nPONGCOMMAND : execute - start" << RESET << std::endl;
	std::string message = arg.at(0);

	// client->writetosend(message);
	//client->reply(RPL_PING(client->getPrefix(), message));
	client->reply_command(RPL_PING(client->getPrefix(), message));
	//std::cout << FUCHSIA << "PONGCOMMAND : execute - end" << RESET << std::endl;
}