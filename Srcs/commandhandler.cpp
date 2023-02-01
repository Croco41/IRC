#include "../Includes/commandhandler.hpp"

CommandHandler::CommandHandler(Server *server)
	: _server(server)
{
	_commands["JOIN"] = new JoinCommand(_server);
}

CommandHandler::~CommandHandler()
{
	for (std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
		delete it->second;
}

void	CommandHandler::recup_msg(Client *client, std::string message)
{
	std::stringstream	ssMsg(message);
	std::string			msg_parse;

	while(std::getline(ssMsg, msg_parse))
	{
		int len = msg_parse.length();
		if (msg_parse[len - 1] == '\r')
			msg_parse = msg_parse.substr(0, len - 1);

		std::string cde_name = msg_parse.substr(0, msg_parse.find(' '));

		try
		{
			Command 					*command = _commands.at(cde_name);
			std::vector<std::string>	arguments;
			std::string 				buf;
			std::stringstream 			ssArg(msg_parse.substr(cde_name.length(), msg_parse.length()));

			while (ssArg >> buf)
				arguments.push_back(buf);

			// bool isRegistered = client->isRegistered();
			// bool authRequired = command->authRequired();
			// if (!isRegistered && authRequired)
			// {
			// 	client->reply(ERR_NOTREGISTERED(client->getNickname()));
			// 	return;
			// }

			command->execute(client, arguments);
		}
		catch (const std::out_of_range &e)
		{
			client->reply(ERR_UNKNOWNCOMMAND(client->getNickname(), cde_name));
		}
	}
}