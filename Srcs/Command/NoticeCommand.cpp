#include "command_list.hpp"

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
	int i = 0;
	while (message.at(i) != ':')
		i++;
	message = message.substr(i + 1);
	// On vérifie si le Client target existe
	std::string target = arg.at(0);
	std::cout << PURPLE << target  << target.at(0) << RESET << std::endl;
	if (target.at(0) != '#')
	{
		Client		*dest = _server->getClient(target);
		if (!dest) // && target[0] != '#')
		{
			std::cout << PURPLE << "client n'existe pas ?" << RESET << std::endl;
			return;
		}
		std::cout << "target= " << target << std::endl;
		std::cout << "message= " << message << std::endl;
		dest->writetosend(RPL_PRIVMSG(client->getPrefix(), target, message));
	}
	// On vérifie si le message est envoyé sur un channel (commence par #)
	if (target.at(0) == '#')
	{
		if (client->findChannel(target) == true)
		{
			std::cout << "j'arrive jusqu'ici" << std::endl;
			Channel		*channel = _server->getChannel(target);
			channel->sendall(RPL_NOTICE(client->getPrefix(), target, message));
		}
		else
		{
			std::cout << PURPLE << "pas dans le chan" << RESET << std::endl;
		}
	}	
	std::cout << FUCHSIA << "NOTICECOMMAND : execute - end" << RESET << std::endl;
}