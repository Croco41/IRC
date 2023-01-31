#include "../../Includes/command_list.hpp"

JoinCommand::JoinCommand(Server *server) : Command(server)
{
	return;
}

JoinCommand::~JoinCommand()
{
	return;
}

void JoinCommand::execute(Client *client, std::vector<std::string> arg)
{
	// on va d'abord vérifier si les info de creation ou de connexion au
	// channel sont correctes, et ensuite on laisse le client join le channel
	
	// verif si le tableau d'arg est bien rempli
	if (arg.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"));
		return;
	}
	// on établit le mot de passe (soit arg 2, soit rien)
	std::string	chan_name = arg[0]; 
	std::string	password;
	if (arg.size() > 1)
		password = arg[1];
	else
		password = "";
	// on verifie si le client est déjà connecté
	Channel		*channel = client->getChannel();
	if (channel)
	{
		client->reply(ERR_TOOMANYCHANNELS(client->getNickname(), chan_name));
		return;
	}
	// on crée le channel et on vérifie s'il peut encore accepter des clients
	channel = _server->getChannel(chan_name);
	if (!channel)
		channel = _server->createChannel(chan_name, password, client);
	if (channel->getMaxclients() > 0 && channel->getNbclients())
	{
		client->reply(ERR_CHANNELISFULL(client->getNickname(), chan_name));
		return;
	}
	// verif du mot de passe
	if (channel->getPassword() != password)
	{
		client->reply(ERR_BADCHANNELKEY(client->getNickname(), chan_name));
		return;
	}

	client->join_channel(channel);
}
