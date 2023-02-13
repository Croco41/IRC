#include "../../Includes/command_list.hpp"

OperCommand::OperCommand(Server *server) : Command(server)
{
	return;
}

OperCommand::~OperCommand()
{
	return;
}

void OperCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nOperCOMMAND : execute - start" << RESET << std::endl;
	std::string name = "Isabelle";
	std::string password = "Cyrielle";
	if (arg.size() == 2)
	{
		std::string tryname = arg.at(0);
		std::string trypass = arg.at(1);
		if (tryname != name || trypass != password)
		{
			client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
		}
		else if (name == tryname && password == trypass)
		{
			client->setModes(client->getModes() + "+o");
			client->setOperator(1);
			std::cout << YELLOW << " modes: " << client->getModes() << RESET << std::endl;
			client->reply(RPL_YOUREOPER(client->getNickname()));
			client->reply_command(RPL_MODE(client->getPrefix(), client->getNickname(), client->getModes(), ""));
	}
	else
	{
		 client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "OPER"));
		 return;
	}
	std::cout << FUCHSIA << "OperCOMMAND : execute - end" << RESET << std::endl;
}