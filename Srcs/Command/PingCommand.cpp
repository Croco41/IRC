#include "../../Includes/command_list.hpp"

PingCommand::PingCommand(Server *server) : Command(server)
{
	return;
}

PingCommand::~PingCommand()
{
	return;
}

void PingCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << DARKMAGENTA << "On arrive dans Ping execute ?" << RESET << std::endl;
	std::string message = arg.at(0);

	// client->writetosend(message);
	client->reply(RPL_PING(client->getPrefix(), message));
}