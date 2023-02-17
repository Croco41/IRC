#include "command_list.hpp"

PartCommand::PartCommand(Server *server) : Command(server)
{
	return;
}

PartCommand::~PartCommand()
{
	return;
}

// PART [<channels>] [<message>]
// Cette commande permet de quitter un channel (1er arg) et d'envoyer un message de départ (2e arg)
void PartCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nPARTCOMMAND : execute - start" << RESET << std::endl;
	// std::cout << "YOUHOU PART !!" << client->getChannel()->getName() << std::endl;
	for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
		std::cout << "arg[" << *it << "]" << std::endl;
	// Au moins un paramètre obligatoire : le channel qu'on veut quitter !
	if (arg.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
		return;
	}

// parsing des noms de channels !

	std::vector<std::string>	chan;
	std::stringstream			ssChan(arg[0]);
	std::string					each_chan;

	while (std::getline(ssChan, each_chan, ','))
		chan.push_back(each_chan);

	for (std::vector<std::string>::iterator it = chan.begin(); it != chan.end(); it++)
	{
		std::cout << GREEN << "CHANNEL EN COURS : " << *it << RESET << std::endl;
		if (it.operator*().at(0) != '#')
		{
			std::cout << GREEN << "ON ENTRE ICI # ?" << it.operator*().at(0) << RESET << std::endl;
			std::string	tmp = *it;
			*it = "#";
			it.operator*().append(tmp);
			std::cout << GREEN << *it << RESET << std::endl;
		}
		std::string	chan_message = "";
		if (arg.size() == 2)
			chan_message = arg[1];
		Channel		*channel = _server->getChannel(*it);
		if (!channel)
		{
			std::cout << ORANGE << "channel empty ? Normalement non !" << RESET << std::endl;
			client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), *it));
			return;
		}
		if (!client->findChannel(*it))
		{
			client->reply(ERR_NOTONCHANNEL(client->getNickname(), *it));
			return;
		}
		// Si le channel existe et le client est bien membre : on le quitte !
		client->leave_channel(channel, chan_message, 0);
		if (_server->getChannel(*it)->getNbclients() == 0)
			_server->destroyChannel(channel);
		else
			std::cout << "nb de clients dans notre channel (PART) " << channel->getNbclients() << std::endl;
	
	}

	std::cout << FUCHSIA << "PARTCOMMAND : execute - end" << RESET << std::endl;
}




