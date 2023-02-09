#include "../../Includes/command_list.hpp"

NickCommand::NickCommand(Server *server) : Command(server)
{
	return;
}

NickCommand::~NickCommand()
{
	return;
}

// syntax : /nick <new nick> -> 1 paramètre obligatoire !
void NickCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nNICKCOMMAND : execute - start" << RESET << std::endl;
	if (arg.size() < 1)
	{
		client->reply(ERR_NONICKNAMEGIVEN(client->getNickname(), "NICK"));
		return;
	}

	std::string reply = client->getNickname();
	std::string nickname = arg[0];
	if (_server->getClient(nickname))
	{
		client->reply(ERR_NICKNAMEINUSE(client->getNickname()));
		return;
	}

	client->setNickname(nickname);
	reply.append(" changed his nickname to ");
	reply.append(client->getNickname()); 
	client->reply_command(reply);
	std::cout << FUCHSIA << "NICKCOMMAND : execute - end" << RESET << std::endl;
}