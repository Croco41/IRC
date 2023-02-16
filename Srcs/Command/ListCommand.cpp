#include "../../Includes/command_list.hpp"

ListCommand::ListCommand(Server *server) : Command(server)
{
	return;
}

ListCommand::~ListCommand()
{
	return;
}

// syntax : /LIST <channel> ou /LIST <liste de channels
void ListCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nLISTCOMMAND : execute - start" << RESET << std::endl;
	/*
	/list affiche tous les channels existant sur le réseau IRC avec quelques conditions
	*/
	if (arg.size() > 1)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "LIST"));
		return;
	}
	std::vector<Channel *> all_chans = _server->getChannels();
	if (!all_chans.size())
	{
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), "LIST"));
		return;
	}
	if (arg.empty()) // affiche tous les channels du serveur
	{
		for (std::vector<Channel *>::iterator it = all_chans.begin(); it != all_chans.end(); it++)
		{
			int n = it.operator*()->getNbclients();
			std::stringstream	ss;
			ss << n;
			std::string			nbclients = ss.str();
			client->reply_command(RPL_LIST(client->getNickname(), it.operator*()->getName(), nbclients, it.operator*()->getTopic()));
		}
		client->reply_command(RPL_LISTEND(client->getNickname()));
	}
	if (arg.at(0)[0] == '#') // affiche une liste des channels specifies (avec parsing)
	{
		std::vector<std::string>	chan;
		std::stringstream			ssChan(arg.at(0));
		std::string					each_chan;

		while (std::getline(ssChan, each_chan, ','))
			chan.push_back(each_chan);

		for (std::vector<std::string>::iterator it = chan.begin(); it != chan.end(); it++)
		{
			if (it.operator*().at(0) != '#')
			{
				std::string	tmp = *it;
				*it = "#";
				it.operator*().append(tmp);
			}
			Channel		*channel = _server->getChannel(*it);
			if (!channel)
			{
				client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), arg.at(0)));
				return;
			}
			int n = channel->getNbclients();
			std::stringstream	ss;
			ss << n;
			std::string			nbclients = ss.str();
			client->reply_command(RPL_LIST(client->getNickname(), channel->getName(), nbclients, channel->getTopic()));
		}
		client->reply_command(RPL_LISTEND(client->getNickname()));
	}
	else // on a un parametre qui est une string, on verifie si on la trouve dans un topic et on liste les channels qui ont ce topic !
	{
		for (std::vector<Channel *>::iterator it = all_chans.begin(); it != all_chans.end(); it++)
		{
			if (it.operator*()->getTopic().find(arg.at(0)) != std::string::npos)
			{
				int n = it.operator*()->getNbclients();
				std::stringstream	ss;
				ss << n;
				std::string			nbclients = ss.str();
				client->reply_command(RPL_LIST(client->getNickname(), it.operator*()->getName(), nbclients, it.operator*()->getTopic()));
			}
		}
		client->reply_command(RPL_LISTEND(client->getNickname()));
	}
	std::cout << FUCHSIA << "NAMECOMMAND : execute - end" << RESET << std::endl;
}