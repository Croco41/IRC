#include "command_list.hpp"

WhoisCommand::WhoisCommand(Server *server) : Command(server)
{
	return;
}

WhoisCommand::~WhoisCommand()
{
	return;
}

// syntax : /WHOIS <nickname>
void WhoisCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << DARKVIOLET << "contenu du vecteur arg : " << std::endl;
	for(std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
		std::cout << *it << std::endl;
	std::cout << RESET << std::endl;
	if (arg.empty() || arg.size() > 1) // 1 seul param !
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "WHOIS"));
		return;
	}
	Client		*target = _server->getClient(arg.at(0));
	if (!target)
	{
		client->reply(ERR_NOSUCHNICK(client->getNickname(), "WHO"));
		return;
	}
	client->reply(RPL_WHOISUSER(target->getNickname(), target->getRealname()));
	client->reply(RPL_WHOISSERVER(target->getNickname(), _server->getServname()));
	if (target->getModes().find('o') != std::string::npos)
		client->reply(RPL_WHOISOPERATOR(target->getNickname()));
	if (target->getChannel().empty() == false)
		client->reply_command(RPL_WHOISCHANNELS(target->getNickname(), target->getListChannel()));
	client->reply_command(RPL_ENDOFWHOIS(client->getRealname()));
}