#include "../../Includes/command_list.hpp"

NickCommand::NickCommand(Server *server) : Command(server)
{
	return;
}

NickCommand::~NickCommand()
{
	return;
}

bool checkNickInvalid(std::string nickname)
{
	int i = 0;
	int length = nickname.length();

	//La taille du nick ne doit pas depasser 9 idealement
	if (length > 9)
	{
		std::cout << "NICK Error: Nickname is too big (>9)" << std::endl;
		return (false);
	}
	while (i < length)
	{
		if (!std::strchr(NICK_VALID_CHARS, nickname[i]))
		{
			std::cout << "NICK Error: Found special characters" << std::endl;
			return (false);
		}
		i++;
	}
	if (!isalpha(nickname[0]))
	{
		std::cout << "NICK Error: The nickname does not start with an alpha" << std::endl;
		return (false);
	}
	return true;
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
	if (checkNickInvalid(nickname) == false)
	{
		client->reply(ERR_ERRONEUSNICKNAME(client->getNickname()));
		return;
	}
	if (_server->getClient(nickname))
	{
		client->reply(ERR_NICKNAMEINUSE(client->getNickname()));
		return;
	}
	std::string oldNick = client->getNickname();
	std::string newNick = nickname;
	client->setNickname(nickname);
	reply.append(" changed his nickname to ");
	reply.append(client->getNickname()); 
	client->reply_command(reply);
	client->reply_command(RPL_NICK(oldNick, newNick));


	std::cout << FUCHSIA << "NICKCOMMAND : execute - end" << RESET << std::endl;
}