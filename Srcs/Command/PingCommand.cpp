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
	std::string message = arg.at(0);
	message.append("\r\n");
	write(client->getFd(), message.c_str(), message.length());
}