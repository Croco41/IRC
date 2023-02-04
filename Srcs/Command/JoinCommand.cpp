#include "../../Includes/command_list.hpp"

// hello isa!!

JoinCommand::JoinCommand(Server *server) : Command(server)
{
	return;
}

JoinCommand::~JoinCommand()
{
	return;
}

void JoinCommand::execute(Client *client, std::vector<std::string> arg)
{


