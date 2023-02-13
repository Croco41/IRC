#include "../../Includes/command_list.hpp"

KickCommand::KickCommand(Server *server) : Command(server)
{
	return;
}

KickCommand::~KickCommand()
{
	return;
}

void KickCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nKickCOMMAND : execute - start" << RESET << std::endl;
	if (arg.size() >= 3)
	{
		std::string target = arg.at(0);
		std::string users = "";
		Channel *channel = _server->getChannel(target);
		if(!channel)
		{
			client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), target));
			return;
		}
		else if (channel)
		{
			std::cout << YELLOW << "le chan existe" << RESET << std::endl;
			users = channel->getNicknamesList();
			std::cout << YELLOW << "users" << RESET << std::endl;
		}
		if(channel && (channel->client_is_inchannel(client) == false))
		{
			client->reply(ERR_NOTONCHANNEL(client->getNickname(), target));
			return;
		}
		else if(channel && (channel->client_is_inchannel(client) == true) && (client->getOperator() == false && (client != channel->getAdmin())))
		{
			client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), target));
			return;
		}
		else
		{
			std::vector<std::string>	userskick;
			std::stringstream			ssUserskick(arg[1]);
			std::string					each_user;

			while (std::getline(ssUserskick, each_user, ','))
				userskick.push_back(each_user);
			
			for (std::vector<std::string>::iterator it = userskick.begin(); it != userskick.end(); it++)
			{	
				std::cout << GREEN << "USERS à KICK EN COURS : " << *it << RESET << std::endl;
				std::string	kick_message = "";
				std::string kick = (*it);
				std::cout << YELLOW << arg.size() << RESET << std::endl;
				kick_message = arg[2];
				// else
				// {
				// 	kick_message = client->getNickname() + " kick " + kick + " from this channel: " + channel->getName();
				// 	std::cout << BLUE << kick_message << RESET << std::endl;
				// }
				Client		*clientkick = _server->getClient(*it);
				// si on ne trouve pas le channel que l'on veut quitter : error
				if (clientkick && channel->client_is_inchannel(clientkick) ==true)
				{
					if (clientkick == client)
					{
						std::cout << "You can't kick yourself" << std::endl;
						continue;
					}
					std::cout << GREEN << "On trouve l'user à kick dans le chan !" << RESET << std::endl;
					clientkick->leave_channel(channel, "");
					channel->setNbclients(channel->getNbclients() - 1);
					channel->sendall(RPL_KICK(client->getPrefix(), channel->getName(), kick, kick_message));
				}
				else
				{
					std::cout << ORANGE << "user " << (*it) << " is not on the chan" << RESET << std::endl;
					client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), (*it), channel->getName()));
				}

			}
		}
	}
	else
	{
		 client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "Kick"));
		 return;
	}
	std::cout << FUCHSIA << "KickCOMMAND : execute - end" << RESET << std::endl;
}