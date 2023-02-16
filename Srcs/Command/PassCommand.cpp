#include "command_list.hpp"

PassCommand::PassCommand(Server *server) : Command(server)
{
	return;
}

PassCommand::~PassCommand()
{
	return;
}

// syntax : /Pass <Pass> -> 1 paramètre obligatoire !
void PassCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nPassCOMMAND : execute - start" << RESET << std::endl;
	int param_size = arg.size();
	if (param_size < 1)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "Pass"));
		return;
	}
	else if (client->getRegistered() == 1)
	{
		client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
		return;
	}
	else
	{
		std::cout << RED << "ATTENTION: on est dans PassCommand, on set le password avant l'enregistrement!" << RESET << std::endl;
		//cette partie à utiliser si on enlève le parsing du début
		std::string Pass = arg[0];
		std::cout << Pass << std::endl;
		client->setPassword(Pass);
	}
	if (client->getPassword() != "" && client->getPassword() != _server->getPassword())
	{
		client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
		//_server->onClientDisconnect(client->getFd(), _server->getEpollfd());
		_server->setErrorPass(1);
		return;
	}

	std::cout << FUCHSIA << "PassCOMMAND : execute - end" << RESET << std::endl;
}