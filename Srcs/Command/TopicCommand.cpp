#include "command_list.hpp"

TopicCommand::TopicCommand(Server *server) : Command(server)
{
	return;
}

TopicCommand::~TopicCommand()
{
	return;
}

// Clients joining the channel in the future will receive a RPL_TOPIC numeric (or lack thereof) accordingly. FAIRE CA!!!!!!
void TopicCommand::execute(Client *client, std::vector<std::string> arg)
{
	std::cout << FUCHSIA << "\nTOPICCOMMAND : execute - start" << RESET << std::endl;
	std::string target = arg.at(0);
	Channel *channel = _server->getChannel(target);
	if(channel)
	{
		std::string modesOn = channel->getModes();
		if(arg.size() == 1)
		{
			//std::cout << YELLOW << " topic: " << channel->getTopic() << "fin" << RESET << std::endl;
			if(channel->getTopic() == "")
			{
				//std::cout << "je suis dans topic unset" << std::endl;
				client->reply(RPL_NOTOPIC(client->getNickname(), channel->getName()));
			}
			else
			{
				client->reply(RPL_TOPIC(client->getNickname(), channel->getName(), channel->getTopic()));
			}	
		}
		else if (arg.size() >= 2)
		{
			std::string topic;
			for (std::vector<std::string>::iterator it = arg.begin()++; it != arg.end(); it ++)
			{
				topic.append(*it);
				topic.append(" ");
			}
			if(modesOn.find("+t") != std::string::npos && (client->getOperator() == false && (client != channel->getAdmin()))) 
			{
				client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()));
			}
			else
			{
				std::cout << YELLOW << " topic: " << topic << "fin" << RESET << std::endl;
				if(topic == ":")
				{
					topic = "";
					channel->setTopic(topic);
					client->reply_command(RPL_NOTOPIC(client->getPrefix(), channel->getName()));
					std::cout << YELLOW << " topic unset: " << channel->getTopic() << RESET << std::endl;
				}
				else
				{
					channel->setTopic(topic);
					std::cout << YELLOW << " topic: " << channel->getTopic() << RESET << std::endl;
					client->reply_command(RPL_TOPIC(client->getPrefix(), channel->getName(), channel->getTopic()));
				}
				channel->sendall(RPL_TOPICALL(client->getPrefix(), channel->getName(), channel->getTopic()));
			}
		}
		else
		{
			client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "OPER"));
			return;
		}
	}
	else
	{
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), target));
	}
	
	std::cout << FUCHSIA << "TOPICCOMMAND : execute - end" << RESET << std::endl;
}