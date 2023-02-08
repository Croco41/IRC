#include "../../Includes/command_list.hpp"

// hello isa!!

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
	std::cout << FUCHSIA << "\nJOINCOMMAND : execute - start" << RESET << std::endl;

	// on va d'abord vérifier si les info de creation ou de connexion au
	// channel sont correctes, et ensuite on laisse le client join le channel
	
	// verif si le tableau d'arg est bien rempli
	if (arg.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"));
		return;
	}
	for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
		std::cout << "arg[" << *it << "]" << std::endl;
	// on établit le mot de passe (soit arg 2, soit rien)
	std::string	chan_name = arg[0]; 
	std::string	password;
	//bon...
	std::cout << KHAKI << "password" << RESET << std::endl;
	if (arg.size() > 1)
		password = arg[1];
	else
		password = "";
	// on verifie si le client est déjà connecté
	std::cout << KHAKI << "client déjà connecté ?" << client->findChannel(chan_name) << RESET << std::endl;

	if (client->findChannel(chan_name) == true)
	{
		std::cout << RED << "On entre ici ?" << RESET << std::endl;
		client->reply(ERR_TOOMANYCHANNELS(client->getNickname(), chan_name));
		return;
	}
	// on crée le channel et on vérifie s'il peut encore accepter des clients
	std::cout << KHAKI << "création du channel" << RESET << std::endl;
	Channel		*channel = _server->getChannel(chan_name);
	if (!channel)
		channel = _server->createChannel(chan_name, password, client);
	if (channel->getMaxclients() > 0 && channel->getNbclients() >= channel->getMaxclients())
	{
		client->reply(ERR_CHANNELISFULL(client->getNickname(), chan_name));
		return;
	}
	// verif du mot de passe
	std::cout << KHAKI << "verif password" << RESET << std::endl;
	if (channel->getPassword() != password)
	{
		client->reply(ERR_BADCHANNELKEY(client->getNickname(), chan_name));
		return;
	}
	std::cout << KHAKI << "juste avant de join_channel" << RESET << std::endl;
	client->join_channel(channel);
	std::cout << "nb de clients dans notre channel (JOIN) " << channel->getNbclients() << std::endl;
	std::cout << FUCHSIA << "JOINCOMMAND : execute - end" << RESET << std::endl;
}
