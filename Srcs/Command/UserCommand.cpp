#include "command_list.hpp"

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
	if (param_size < 3)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
		return;
	}
	else if (client->getRegistered() == 1)
	{
		client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
		return;
	}
	// else if (client->getPassword() != _server->getPassword())
	// {
	// 	client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
	// 	//_server->onClientDisconnect(client->getFd(), _server->getEpollfd());
	// 	_server->setErrorPass(1);
	// 	return;
	// }
	else
	{
		std::string message;
		std::string realname;
		std::string username = arg[0];
		client->setUsername(username);
		
		for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); ++it) 
		{
			std::string tosend; 
			tosend = (*it); 
			message.append(" ");
			message.append(tosend);
		}
		message.append("\t\n");
		std::cout << message << std::endl;
		size_t start;
		size_t end;
		start = message.find(":");
		if(message.find(":") == std::string::npos)
		{
			realname = arg[2];
			std::cout << start << std::cout;
		}
		else
		{
			end = message.find_first_of("\t\n", start);
			realname = message.substr(start + 1, end - start -1);
		}
		std::cout << RED << "ATTENTION: on est dans UserCommand, on set les params!" << RESET << std::endl;
		client->setRealname(realname);
		client->setRegistered(1);
	}

	std::cout << FUCHSIA << "USERCOMMAND : execute - end" << RESET << std::endl;
}