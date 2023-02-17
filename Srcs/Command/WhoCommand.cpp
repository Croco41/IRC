#include "command_list.hpp"

WhoCommand::WhoCommand(Server *server) : Command(server)
{
	return;
}

WhoCommand::~WhoCommand()
{
	return;
}

// syntax : /WHO <mask>
void WhoCommand::execute(Client *client, std::vector<std::string> arg)
{
	/*
	/La commande /who permet d'afficher des informations sur les utilisateurs. 
	Sans argument, cela retourne la liste des personnes connectées sur le réseau. 
	Si arg = channel : liste les user non invisibles du channel !
	si arg = user : donne les info sur le user !
	 */
	// std::cout << DARKVIOLET << "contenu du vecteur arg : " << std::endl;
	// for(std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
	// 	std::cout << *it << std::endl;
	// std::cout << RESET << std::endl;
	if (arg.empty()) // donner les noms des users du serveur !
	{
		std::map<int, Client *>	serv_clients = _server->getClients();
		for (std::map<int, Client *>::iterator it = serv_clients.begin(); it != serv_clients.end(); it++)
			it.operator*().second->reply_command(RPL_WHOREPLY(_server->getServname(), it.operator*().second->getUsername())); // ici : serveur, nb de serveurs intermediaires, nom du client sur le serveur
	}
	else if (arg.size() > 1)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "WHO"));
		return;
	}
	else if (arg.size() < 2 && arg.at(0)[0] == '#') // donner les noms des users du channel !
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
			std::string str;
			Client *target = *it;
			str += channel->getName() + " ";
			str += target->getUsername() + " ";
			str += target->getHostname() + " ";
			str += _server->getServname() + " ";
			str += target->getNickname() + " ";
			str += "H";
			if (target->getOperator() == 1)
						str += "*";
			str += std::string(":0") + " ";
			str += target->getRealname();
		
			//client->reply_command(RPL_WHOREPLY(client->getNickname(), str));
			client->reply_command(RPL_WHOREPLY(client->getNickname(), str)); // ici : channel, nb de serveurs, nom du client
		}
	}
	else if (arg.size() < 2 && arg.at(0)[0] != '#')
	{
		std::cout << DARKVIOLET << "On entre ici ?" << RESET << std::endl;
		Client *target = _server->getClient(arg.at(0));
		if (!target)
		{
			client->reply(ERR_NOSUCHNICK(client->getNickname(), "WHO"));
			return;
		}
		std::string str;

		str += std::string("*") + " ";
		str += target->getUsername() + " ";
		str += target->getHostname() + " ";
		str += _server->getServname() + " ";
		str += target->getNickname() + " ";
		str += "H";
		if (target->getOperator() == 1)
					str += "*";
		str += std::string(":0") + " ";
		str += target->getRealname();
		
		client->reply_command(RPL_WHOREPLY(client->getNickname(), str)); // ici :user, nb de serveurs, nom du client
	}
	std::string mask;
	mask.append(arg[0]);
	client->reply_command(RPL_ENDOFWHO(client->getNickname(), mask));
}

