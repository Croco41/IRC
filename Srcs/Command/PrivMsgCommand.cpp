#include "command_list.hpp"

PrivMsgCommand::PrivMsgCommand(Server *server) : Command(server)
{
	return;
}

PrivMsgCommand::~PrivMsgCommand()
{
	return;
}

// ATTENTION !!!!!!!!!!! Si tu modifies ! 		
// PENSEZ a mettre à jour NOTICE si PRIVMSG modifié			

void PrivMsgCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nPRIVMSGCOMMAND : execute - start" << RESET << std::endl;

	std::cout << DARKVIOLET << "contenu du vecteur arg : " << std::endl;
	for(std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
		std::cout << *it << std::endl;
	std::cout << RESET << std::endl;

	// PRIVMSG prend tjs 2 arg : la cible du Message (channel ou autre client), et le Message lui-même. Verif :
	if (arg.size() < 2 || arg[0].empty() || arg[1].empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
		return;
	}
	// on reconstruit le message integral avec le vector arg, séparées par un espace
	std::string	message;
	for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
		message.append(*it + " ");
	// si le message commence par ':' -> on élague le ':' ! (car les usernames ont tjs un : devant...)
	//std::cout << "message apres append " "= " << message << std::endl;
	int i = 0;
	while (message.at(i) != ':')
		i++;
	message = message.substr(i + 1);
	std::cout << "message apres if := " << message << std::endl;
	// On vérifie si le Client target existe

// VERIF DE LA NATURE DE L'ARG 0 : client ou channel ?


	std::string target = arg.at(0);
	std::cout << PURPLE << target  << target.at(0) << RESET << std::endl;
	if (target.at(0) != '#')
	{
		Client		*dest = _server->getClient(target);
		if (!dest) // && target[0] != '#')
		{
			std::cout << PURPLE << "On est ici ?" << RESET << std::endl;
			client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
			return;
		}
		std::cout << "target= " << target << std::endl;
		std::cout << "message= " << message << std::endl;
		dest->writetosend(RPL_PRIVMSG(client->getPrefix(), target, message));
	}
	// On vérifie si le message est envoyé sur un channel (commence par #)
	// if (target.at(0) == '#')
	else
	{
		// on récupère le channel via le Client et on vérifie qu'il existe !

		if (client->findChannel(target) == true)
		{
			std::cout << "j'arrive jusqu'ici" << std::endl;
			Channel		*channel = _server->getChannel(target);
			channel->sendall(RPL_PRIVMSG(client->getPrefix(), target, message));
		}
		else
		{
			std::cout << PURPLE << "On est la ?" << RESET << std::endl;
			client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), target));
		}
	}	
	std::cout << "target= " << target << std::endl;
	std::cout << "message= " << message << std::endl;
	// dest->writetosend(RPL_PRIVMSG(client->getPrefix(), target, message));
	std::cout << FUCHSIA << "\nPRIVMSGCOMMAND : execute - start" << RESET << std::endl;
}