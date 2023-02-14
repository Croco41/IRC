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
	
	if (arg.empty() || (arg.size() < 2 && arg.at(0)[0] == '#')) // donner les noms des users du channel !
	{
		Channel		*channel = _server->getChannel(arg.at(0));
		if (!channel)
		{
			client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), arg.at(0)));
			return;
		}
		std::vector<Client *> chan_clients = _server->getChannel(arg.at(0))->getClients();
		std::cout << DARKVIOLET << arg.size() << RESET << std::endl;
		for (std::vector<Client *>::iterator it = chan_clients.begin(); it != chan_clients.end(); it++)
		{
			if (it.operator*()->getModes().find('i') != std::string::npos)
				continue;
			it.operator*()->reply(RPL_WHOREPLY(client->getNickname(), arg.at(0), it.operator*()->getPrefix(), it.operator*()->getRealname()));
		}

	}
	else if (arg.size() < 2 && arg.at(0)[0] != '#')
	{
		Client *client = _server->getClient(arg.at(0));
		if (!client)
		{
			client->reply(ERR_NOSUCHNICK(client->getNickname(), "WHO"));
			return;
		}
		client->reply_command(RPL_WHOREPLY(arg.at(0), client->getPrefix(), client->getRealname()));
	}
	client->reply_command(RPL_ENDOFWHO(client->getRealname()));
}