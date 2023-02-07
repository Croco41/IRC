#include "../../Includes/command_list.hpp"

PrivMsgCommand::PrivMsgCommand(Server *server) : Command(server)
{
	return;
}

PrivMsgCommand::~PrivMsgCommand()
{
	return;
}

void PrivMsgCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << "je rentre dans execute privmes" << std::endl;
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
	std::cout << "message apres append " "= " << message << std::endl;

	int i = 0;
	while (message.at(i) != ':')
		i++;
	message = message.substr(i + 1);

	std::cout << "message apres if := " << message << std::endl;
	// On vérifie si le Client target existe
	std::string target = arg.at(0);
	Client		*dest = _server->getClient(target);
	if (!dest)
	{
		client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
		return;
	}
	// On vérifie si le message est envoyé sur un channel (commence par #)
	if (target.at(0) == '#')
	{
		// on récupère le channel via le Client !
		Channel *channel = client->getChannel();
		// on vérifie d'abord si le channel existe
		if (!channel)
		{
			client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), target));
			return;
		}
		// on cherche le bon destinataire dans la liste des clients connectés à ce channel
		std::vector<std::string>			nicknames(channel->getNicknames());
		std::vector<std::string>::iterator	it;

		for (it = nicknames.begin(); it != nicknames.end(); it++)
			if (*it == client->getNickname())
				break;
		// si on ne trouve pas de correspondant :
		if (it == nicknames.end())
		{
			client->reply(ERR_CANNOTSENDTOCHAN(client->getNickname(), target));
			return;
		}
		channel->sendall(message, client);
		return;
	}
	std::cout << "target= " << target << std::endl;
	std::cout << "message= " << message << std::endl;
	dest->writetosend(RPL_PRIVMSG(client->getPrefix(), target, message));
}