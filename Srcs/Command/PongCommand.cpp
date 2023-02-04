#include "../../Includes/command_list.hpp"

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
	std::cout << DARKMAGENTA << "On arrive dans Pong execute ?" << RESET << std::endl;
	std::string message = arg.at(0);

	// client->writetosend(message);
	client->reply(RPL_PING(client->getPrefix(), message));
}