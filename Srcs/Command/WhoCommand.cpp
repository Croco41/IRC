#include "../../Includes/command_list.hpp"

WhoCommand::WhoCommand(Server *server) : Command(server)
{
	return;
}

WhoCommand::~WhoCommand()
{
	return;
}

void WhoCommand::execute(Client *client, std::vector<std::string> arg)
{
	/*
	/La commande /who permet d'afficher des informations sur les utilisateurs. 
	Sans argument, cela retourne la liste des personnes connectées sur le réseau. 
	Tu peux néanmoins spécifier un channel ou un mask pour nickname, usernamer ou hostname.
	Enfin, tu peux lister les IRCOperators en spécifiant o en deuxième argument.
	 */
	// Le premier terme d'arg est toujours le nom du channel dans lequel on lance /who !
	if (arg.size() < 2) // donner les noms des users du channel !
	{
		std::cout << DARKVIOLET << arg.size() << " | " << _server->getChannel(arg.at(0))->getName() << " | " << _server->getChannel(arg.at(0))->getNicknames().at(0) << RESET << std::endl;
		// std::string chan_users;
		std::vector<std::string>::iterator it = _server->getChannel(arg.at(0))->getNicknames().begin();
		std::vector<std::string>::iterator ite = _server->getChannel(arg.at(0))->getNicknames().end();
		// for (; it != ite; it++)
		// int i = 0;
		// for (std::vector<std::string>::iterator it = _server->getChannel(arg.at(0))->getNicknames().begin(); it != _server->getChannel(arg.at(0))->getNicknames().end(); it++)
		// {
		// 	// std::cout << DARKVIOLET << *it << RESET << std::endl;
		// 	chan_users.append(*it);
		// 	// i++;
		// 	std::cout << DARKVIOLET << chan_users << RESET << std::endl;
		// }
		for (; it != ite; it++)
			client->reply_command(RPL_WHOREPLY(client->getPrefix(), arg.at(0), *it));
			// client->reply(RPL_WHOREPLY(client->getNickname(), arg.at(0), chan_users));
		return;
	}
}