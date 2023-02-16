#include "../../Includes/command_list.hpp"

NamesCommand::NamesCommand(Server *server) : Command(server)
{
	return;
}

NamesCommand::~NamesCommand()
{
	return;
}

// syntax : /NAME <channel>
void NamesCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nNAMECOMMAND : execute - start" << RESET << std::endl;
	/*
	/names affiche les nicks des utilisateurs du channel spécifié (les visibles !). Sans channel spécifié, cela retourne tous les nicks de tous les channels.
	*/
	if (arg.empty()) // affiche les nicks des users visibles de tous les channels
	{
		std::vector<Channel *>	serv_chans = _server->getChannels();
		for (std::vector<Channel *>::iterator it = serv_chans.begin(); it != serv_chans.end(); it++)
		{
			std::vector<Client *> chan_clients = it.operator*()->getClients();
			for (std::vector<Client *>::iterator itc = chan_clients.begin(); itc != chan_clients.end(); itc++)
			{
				if (itc.operator*()->getModes().find('i') != std::string::npos)
					continue;
				client->reply_command(RPL_NAMREPLY(client->getNickname(), it.operator*()->getName(), itc.operator*()->getNickname()));
			}
		}
	}
	else if (arg.size() < 2 && arg.at(0)[0] == '#') // affiche les nicks des users visibles du channel specifie
	{
		Channel		*channel = _server->getChannel(arg.at(0));
		if (!channel)
		{
			client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), arg.at(0)));
			return;
		}
		std::vector<Client *> chan_clients = channel->getClients();
		for (std::vector<Client *>::iterator itc = chan_clients.begin(); itc != chan_clients.end(); itc++)
		{
			if (itc.operator*()->getModes().find('i') != std::string::npos)
				continue;
			client->reply_command(RPL_NAMREPLY(client->getNickname(), channel->getName(), itc.operator*()->getNickname()));
		}
	}
	else
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "NAME"));
		return;
	}
	std::cout << FUCHSIA << "NAMECOMMAND : execute - end" << RESET << std::endl;
}