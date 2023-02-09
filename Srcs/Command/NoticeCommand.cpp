#include "../../Includes/command_list.hpp"

// NOTICE message

//      Command: NOTICE
//   Parameters: <target>{,<target>} <text to be sent>

// The NOTICE command is used to send notices between users, 
// as well as to send notices to channels. <target> is interpreted the same way
//  as it is for the Notice command.

// The NOTICE message is used similarly to Notice. 
// The difference between NOTICE and Notice is that automatic replies
//  must never be sent in response to a NOTICE message. 
//  This rule also applies to servers – they must not send any error back 
//  to the client on receipt of a NOTICE command.
//   The intention of this is to avoid loops between a client automatically sending something in response 
//   to something it received. This is typically used by ‘bots’
//    (a client with a program, and not a user, controlling their actions)
//     and also for server messages to clients.

// One thing for bot authors to note is that the NOTICE message
//  may be interpreted differently by various clients. 
//  Some clients highlight or interpret any NOTICE sent to a channel in the same way that a Notice
//   with their nickname gets interpreted. This means that users may be irritated 
//   by the use of NOTICE messages rather than Notice messages by clients or bots, 
//   and they are not commonly used by client bots for this reason.

NoticeCommand::NoticeCommand(Server *server) : Command(server)
{
	return;
}

NoticeCommand::~NoticeCommand()
{
	return;
}

void NoticeCommand::execute(Client *client, std::vector<std::string> arg)
{
		std::cout << FUCHSIA << "\nNOTICECOMMAND : execute - start" << RESET << std::endl;
	// Notice prend tjs 2 arg : la cible du Message (channel ou autre client), et le Message lui-même. Verif :
	if (arg.size() < 2 || arg[0].empty() || arg[1].empty())
	{
		return;
	}
	// on reconstruit le message integral avec le vector arg, séparées par un espace
	std::string	message;
	for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
		message.append(*it + " ");
	// si le message commence par ':' -> on élague le ':' ! (car les usernames ont tjs un : devant...)
	std::cout << "message notice apres append " "= " << message << std::endl;

	int i = 0;
	while (message.at(i) != ':')
		i++;
	message = message.substr(i + 1);

	std::cout << "message notice apres if := " << message << std::endl;
	// On vérifie si le Client target existe
	std::string target = arg.at(0);
	Client		*dest = _server->getClient(target);
	if (!dest)
	{
		return;
	}
	// On vérifie si le message est envoyé sur un channel (commence par #)
	if (target.at(0) == '#')
	{
		// on récupère le channel via le Client et on vérifie qu'il existe !
		std::vector<Channel *>::iterator	it;
		for (it = client->getChannel().begin(); it != client->getChannel().end(); it++)
		{
			if (it.operator*()->getName() == target)
			{
				Channel *channel = *it;
				if (!channel)
				{
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
					return;
				}
				channel->sendall(message, client);
				return;
			}
		}
	}
	std::cout << "target= " << target << std::endl;
	std::cout << "message= " << message << std::endl;
	dest->writetosend(RPL_NOTICE(client->getPrefix(), target, message));
	std::cout << FUCHSIA << "NOTICECOMMAND : execute - end" << RESET << std::endl;
}