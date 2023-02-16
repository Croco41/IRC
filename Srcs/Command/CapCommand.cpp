#include "../../Includes/command_list.hpp"

CapCommand::CapCommand(Server *server) : Command(server)
{
	return;
}

CapCommand::~CapCommand()
{
	return;
}

// syntax : /CAP ls [version] || /CAP REQ
void CapCommand::execute(Client *client, std::vector<std::string> arg)
{
	//on ignore la commande cap
	(void)client;
	(void)arg;
	return;
}