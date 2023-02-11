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
	std::cout << FUCHSIA << "\nPINGCOMMAND : execute - start" << RESET << std::endl;
	std::string message = arg.at(0);

	//client->reply(RPL_PING(client->getPrefix(), message));
	client->reply_command(RPL_PING(client->getPrefix(), message));
	std::cout << FUCHSIA << "PINGCOMMAND : execute - end" << RESET << std::endl;
}