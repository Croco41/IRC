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



	// Le premier terme d'arg est par défaut le nom du channel dans lequel on lance /who !
	// if (arg.size() > 1)
	// {
	// 	client->reply_command(RPL_WHOREPLY(client->getPrefix(), arg.at(0), arg.at(0)));
	// }
	if (arg.empty() || (arg.size() < 2 && arg.at(0)[0] == '#')) // donner les noms des users du channel !
	{
		std::vector<std::string> chan_clients = _server->getChannel(arg.at(0))->getNicknames();
		std::cout << DARKVIOLET << arg.size() << " | " << _server->getChannel(arg.at(0))->getName() << " | " << _server->getChannel(arg.at(0))->getNicknames().at(0) << RESET << std::endl;
		std::string chan_users;
		for (std::vector<std::string>::iterator it = chan_clients.begin(); it != chan_clients.end(); it++)
		{
			chan_users.append(*it);
			chan_users.append(" ");
			std::cout << DARKVIOLET << chan_users << RESET << std::endl;
		}
		client->reply_command(RPL_WHOREPLY(client->getPrefix(), arg.at(0), chan_users));
	}
	else if (arg.size() < 2 && arg.at(0)[0] != '#')
	{
		Client *client = _server->getClient(arg.at(0));
		if (!client)
		{
			client->reply(ERR_NOSUCHNICK(client->getNickname(), "WHO"));
			return;
		}
		client->reply_command(RPL_WHOREPLY(client->getPrefix(), arg.at(0), client->getRealname()));
	}
}