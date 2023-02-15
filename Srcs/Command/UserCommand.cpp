#include "../../Includes/command_list.hpp"

UserCommand::UserCommand(Server *server) : Command(server)
{
	return;
}

UserCommand::~UserCommand()
{
	return;
}

// syntax : /USER <username> <nickname> <hostname> <realname>
void UserCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nUSERCOMMAND : execute - start" << RESET << std::endl;
	int param_size = arg.size();
	if (param_size < 4)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
		return;
	}
	else if (client->getRegistered() == 1)
	{
		client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
		return;
	}
	else
	{
		std::cout << RED << "ATTENTION: on est dans UserCommand, on set les params!" << RESET << std::endl;
		//cette partie à utiliser si on enlève le parsing du début
		std::string username = arg[0];
		//std::string hostname = arg[2]; dans le constructeur
		std::string realname = arg[3];
		client->setUsername(username);
		client->setRealname(realname);	
	}

	std::cout << FUCHSIA << "USERCOMMAND : execute - end" << RESET << std::endl;
}