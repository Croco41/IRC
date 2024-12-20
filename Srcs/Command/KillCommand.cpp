#include "command_list.hpp"

KillCommand::KillCommand(Server *server) : Command(server)
{
	return;
}

KillCommand::~KillCommand()
{
	return;
}

void KillCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nKillCOMMAND : execute - start" << RESET << std::endl;

	if (arg.size() == 2)
	{
		std::string target = arg[0];
		std::cout << YELLOW << target << RESET << std::endl;
		Client *clientkill = _server->getClient(target);
		if(!clientkill)
		{
			client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
			return;
		}
		else if(clientkill && (client->getOperator() == false))
		{
			client->reply(ERR_NOPRIVILEGES(client->getNickname()));
			return;
		}
		else
		{
				std::cout << GREEN << "KILL EN COURS : " << target << RESET << std::endl;
				std::string	kill_message = arg[1];
				std::cout << YELLOW << arg.size() << RESET << std::endl;
				if (clientkill == client)
				{
					std::cout << "You can't kill yourself" << std::endl;
					return;
				}
				std::cout << GREEN << "Faudra coder l'user kill etait dans un chan(operator, client size)!" << RESET << std::endl;
				//vector des channels de l'user a kill
				std::vector<Channel *>	channels_userkill;
				channels_userkill = client->getChannel();
				for (std::vector<Channel *>::iterator it = channels_userkill.begin(); it != channels_userkill.end(); it++)
				{	
					it.operator*()->sendall(RPL_KILL(client->getPrefix(), clientkill->getNickname(), kill_message));
					clientkill->leave_channel((*it), kill_message, 1);
				}
				clientkill->reply_command(RPL_KILL(client->getPrefix(), clientkill->getNickname(), kill_message));
				std::cout << "Client n°" << clientkill->getFd() << " username via serveur: " << clientkill->getNickname() << " va se déconnecter." << std::endl;
				int fd = clientkill->getFd();
				_server->onClientDisconnect(fd, _server->getEpollfd());
			//	std::map<int, Client *> serv_clients = _server->getClients();
			//	delete serv_clients.at(fd);
			//	serv_clients.erase(fd);
			//	close(fd);
				// Log the client disconnection
				std::cout << "Client n°" << fd << " s'est déconnecté." << std::endl;

		}
	}
	else
	{
		 client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "Kill"));
		 return;
	}
	std::cout << FUCHSIA << "KillCOMMAND : execute - end" << RESET << std::endl;
}