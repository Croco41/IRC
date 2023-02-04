#include "../Includes/commandhandler.hpp"

CommandHandler::CommandHandler(Server *server)
	: _server(server)
{
	_commands["JOIN"] = new JoinCommand(_server);
	_commands["PING"] = new PingCommand(_server);
	_commands["PONG"] = new PongCommand(_server);
	_commands["PRIVMSG"] = new PrivMsgCommand(_server);
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
		std::cout << CYAN << "commande = " << cde_name << RESET << std::endl;
		try
		{
			std::cout << TEAL << "on arrive dans le try de CommandHandler::recup_msg" << RESET << std::endl;
			Command 					*command = _commands.at(cde_name);
			// std::cout << INDIANRED << "Command name = " << command << RESET << std::cout;
			std::vector<std::string>	arguments;
			std::string 				buf;
			std::stringstream 			ssArg(msg_parse.substr(cde_name.length(), msg_parse.length()));

			// std::cout << ssArg << std::endl;

			while (ssArg >> buf)
			{
				std::cout << buf << std::endl;
				arguments.push_back(buf);
			}

			std::cout << ROYALBLUE << "buf d'arguments de cde = " << buf << RESET << std::endl;

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
			std::cout << RED;
			client->reply(ERR_UNKNOWNCOMMAND(client->getNickname(), cde_name));
			std::cout << RESET;
		}
	}
}