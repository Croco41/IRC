#include "../../Includes/command_list.hpp"

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
	std::cout << INDIANRED << "execute PART Cmd" << RESET << std::endl;
	std::cout << "YOUHOU PART !!" << client->getChannel() << std::endl;
	// Au moins un paramètre obligatoire : le channel qu'on veut quitter !
	if (arg.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
		return;
	}

	std::string	chan_name = arg[0];
	std::string	chan_message = "";
	if (arg.size() == 2)
		chan_message = arg[1];
	Channel		*channel = _server->getChannel(chan_name);
	// si on ne trouve pas le channel que l'on veut quitter : error
	if (!channel)
	{
		std::cout << ORANGE << "channel empty ? Normalement non !" << RESET << std::endl;
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), chan_name));
		return;
	}
	// si le client n'est pas sur le channel : error
	if (!client->getChannel() || client->getChannel()->getName() != chan_name)
	{
		client->reply(ERR_NOTONCHANNEL(client->getNickname(), chan_name));
		return;
	}
	// Si le channel existe et le client est bien membre : on le quitte !
//	std::cout << ORANGE << "arg[1] = chan message: " << chan_message << RESET << std::endl;
	client->leave_channel(channel, chan_message);
	channel->setNbclients(channel->getNbclients() - 1);
	std::cout << "YOUHOU PART FIN !!" << client->getChannel() << std::endl;
	if (_server->getChannel(chan_name)->getNbclients() == 0)
		_server->destroyChannel(channel);
	else
	{
		std::cout << "nb de clients dans notre channel (PART) " << channel->getNbclients() << std::endl;
	}
}




