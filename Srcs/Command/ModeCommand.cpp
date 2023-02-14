#include "../../Includes/command_list.hpp"

ModeCommand::ModeCommand(Server *server) : Command(server)
{
	return;
}

ModeCommand::~ModeCommand()
{
	return;
}

// 		- a : away// - i : invisible// - w : wallops// - r : restricted// - o : operator
void ModeCommand::mode_user(Client *client, std::vector<std::string> arg)
{
    int arg_size = arg.size();
	std::string modes = client->getModes();
	std::string target = arg.at(0);
	Client *targetclient = _server->getClient(target);
	std::cout << PURPLE << targetclient->getNickname() << RESET << std::endl;
    if (arg_size == 1) //SI JUSTE LA TARGET
    {
        if (targetclient)
        {
            if (client->getNickname() == target)
            {
				std::cout << "dans mode sans arg " << modes << std::endl;
                client->reply(RPL_UMODEIS(client->getNickname(), modes)); //221 To answer a query about a client's own mode, RPL_UMODEIS is sent back.
            }
            else
                client->reply(ERR_USERSDONTMATCH()); 
        }
        else
		{
            client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
		}
        return;
    }
    else if (arg_size == 2) //SI TARGET + MODES TO CHANGE
    {
        if (client->getNickname() == target)
        {
            bool sign = true;
			bool unknown_mode = false;
            std::string modes(MODES_USER);
            for (std::vector<std::string>::iterator it = arg.begin() + 1; it != arg.end(); ++it) 
            {
				std::cout << "value tested debut: " << (*it) << std::endl;
                if (it.operator*().at(0) == '-')
                    sign = false;
                else if (it.operator*().at(0) ==  '+')
                    sign = true;
				std::cout << "value cherchée après + -: " << it.operator*().at(1) << std::endl;
                if (modes.find((it.operator*().at(1))) != std::string::npos)
                {
                    std::string client_modes = client->getModes();
					std::cout << PURPLE << "client_modes: " << client_modes << RESET << std::endl;
                    //if (sign == true && (client_modes.find((*it)) == std::string::npos))
					if ((it.operator*().at(1)) == 'a')
					{
						std::string reply = "The flag 'a' SHALL NOT be toggled using the MODE command,instead use of the AWAY command is REQUIRED";
						client->reply(reply);
						std::cout << RED << reply << RESET << std::endl;
						//client->writetosend(RPL_MODE_AWAY(client->getPrefix(), message));
						//client->reply_command(RPL_MODE_NORIGHT(client->getPrefix(), client->getNickname(), message));
					}
					else if (sign == true && ((it.operator*().at(1)) == 'o' || (it.operator*().at(1)) == 'O'))
					{
						std::string reply = "You need to use OPER command to make an operator";
						client->reply(reply);
						std::cout << RED << reply << RESET << std::endl;
					}
					else if (sign == true && (client_modes.find((it.operator*().at(1))) == std::string::npos)) //add mode to user
                    {
                        std::cout << "add mode" << std::endl;
						client->setModes(client->getModes() + (*it));
						//client->setModes(client->getModes() + (it.operator*().at(0)) + (it.operator*().at(1)));
                    }
					else if (sign == false && ((it.operator*().at(1)) == 'r'))
					{
						std::string reply = "You can't make yourself unrestricted";
						client->reply(reply);
						std::cout << RED << reply << RESET << std::endl;
					}
                    else if (sign == false && (client_modes.find((it.operator*().at(1))) != std::string::npos)) //remove mode to user
                    {
                        std::cout << "remove mode" << std::endl;
						std::string needle;
						needle.push_back((it.operator*().at(1)));
						std::size_t pos = client_modes.find(needle);
						client_modes.erase(pos -1, 2);
						std::cout << GREEN <<  "remove mode, size de new clientmodes (après erase): " << client_modes << client_modes.size() << RESET << std::endl;
                        client->setModes(client_modes);
                    }
					if (sign == false && ((it.operator*().at(1)) == 'o'))
					{
						client->setOperator(0);
					}
                }
                else
				{
                	unknown_mode = true;
					std::cout << RED << "je rentre ici dans unknow mode?? " << RESET << std::endl;
				}
            }
		    if (unknown_mode == true)
            {
                client->reply(ERR_UMODEUNKNOWNFLAG());
            }
			client->reply_command(RPL_MODE(client->getPrefix(), client->getNickname(), client->getModes(), ""));

		}
	}
}

// ATTENTION !!! A mettre à jour dans oper le -o et isoperator du client
void ModeCommand::mode_channel(Client *client, Channel *channel, std::vector<std::string> arg)
{
    int arg_size = arg.size();
	std::string size = "";
	std::string chan_modes_save = channel->getModes();
	std::cout << PURPLE << channel->getName() << RESET << std::endl;
    if (arg_size == 1) //SI JUSTE LA TARGET
    {
        client->reply(RPL_CHANNELMODEIS(channel->getName(), channel->getModes(), size)); //324 To answer a query about channel's mode
        return;
    }
    else if (arg_size == 2) //SI TARGET + MODES TO CHANGE
    {
        bool sign = true;
        std::string modes(CHANNEL_MODES);
		if(channel->client_is_operator(client) == false)
		{
			client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()));
			return;
		}
		else if(channel->client_is_inchannel(client) == false) //a tester
		{
			client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()));
			return;
		}
        for (std::vector<std::string>::iterator it = arg.begin() + 1; it != arg.end(); ++it) 
        {
			std::cout << "value tested debut: " << (*it) << std::endl;
            if (it.operator*().at(0) == '-')
                sign = false;
            else if (it.operator*().at(0) ==  '+')
                sign = true;
			std::cout << "value cherchée après + -: " << it.operator*().at(1) << std::endl;
            if (modes.find((it.operator*().at(1))) != std::string::npos && ((it.operator*().at(1)) == 't'))
            {
                // t =>   The channel flag 't' is used to restrict the usage of the TOPIC
				if(sign == true && chan_modes_save.find((it.operator*().at(1))) == std::string::npos)
				{
					std::cout << "add mode t (restrict usage of the TOPIC)" << std::endl;
					channel->setModes(channel->getModes() + "+t");
					std::cout << YELLOW << channel->getModes() << RESET << std::endl;
				}
				else if (sign == false)
				{
					std::cout << "remove mode t (unrestrict usage of the TOPIC)" << std::endl;
					std::string needle;
					needle.push_back((it.operator*().at(1)));
					if (chan_modes_save.find(needle) != std::string::npos)
					{
						std::size_t pos = chan_modes_save.find(needle);
						std::cout << YELLOW << "chan_modes: " << chan_modes_save << " mode sauv dans chan: " << channel->getModes() << RESET <<std::endl; 
						std::cout << "pour test si on enleve alors qu'y a pas pos= " << pos << std::endl;
						chan_modes_save.erase(pos -1, 2);
						std::cout << YELLOW << "DEBUG: newchanmod after delete: " << chan_modes_save << RESET << std::endl;
						channel->setModes(chan_modes_save);
					}
				}
			}
			else if (sign == false && ((it.operator*().at(1)) == 'l'))
			{
				if (chan_modes_save.find((it.operator*().at(1))) != std::string::npos)
				{
					std::cout << "remove mode l (limite of users in this chan)" << std::endl;
					std::string needle;
					needle.push_back((it.operator*().at(1)));
					if (chan_modes_save.find(needle) != std::string::npos)
					{
						std::size_t pos = chan_modes_save.find(needle);
						chan_modes_save.erase(pos -1, 2);
						std::cout << YELLOW << "DEBUG: new chan_mode after delete: " << chan_modes_save << RESET << std::endl;
						channel->setModes(chan_modes_save);
						channel->setMaxclients(50); //(la valeur max qu'on a fixé sans limite)
					}
				}
			}
			else if (sign == true && ((it.operator*().at(1)) == 'l'))
			{
				std::string reply = "mode +l => besoin d'un parametre: nombre d'utilisateur ";
				client->reply(reply);
				std::cout << RED << reply << RESET << std::endl;
			}
			else if ((it.operator*().at(1)) == 'o')
			{
				std::string reply = "You need to use OPER command to make an operator or Mode user to do -o";
				client->reply(reply);
			}
			else 
			{
				client->reply(ERR_UNKNOWNMODE(client->getNickname(), (*it)));
			}
		}
	}
	else if (arg_size == 3)
	{
		std::cout << "je suis dans channel mode arg3" << std::endl;
		bool sign = true;
		//bool unknown_mode = false;
        std::string modes(CHANNEL_MODES);
		if(channel->client_is_operator(client) == false)
		{
			client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel->getName()));
			return;
		}
		else if(channel->client_is_inchannel(client) == false)
		{
			client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()));
			return;
		}
        for (std::vector<std::string>::iterator it = arg.begin() + 1; it != arg.end(); ++it) 
        {
			std::cout << "value tested debut: " << (*it) << std::endl;
            if (it.operator*().at(0) == '-')
                sign = false;
            else if (it.operator*().at(0) ==  '+')
                sign = true;
			if ((it.operator*().at(1)) == 'o')
			{
				std::string reply = "You need to use OPER command to make an operator (or Mode user to do -o)";
				client->reply(reply);
				break;
			}
			else if ((it.operator*().at(1)) == 'l')
			{
				std::cout << RED << "je suis dans le +l: " << (*it) << RESET << std::endl;
				//++it;
				if (sign == false)
				{
					std::string reply = "ERROR: chan mode -l ne prend pas de paramètre";
					client->reply(reply);
					std::cout << RED << reply << RESET << std::endl;
				}
				else if (sign == true && chan_modes_save.find((it.operator*().at(1))) == std::string::npos)
				{
					++it;
					std::cout << RED << "je suis dans le +l apres ++it: " << (*it) << RESET << std::endl;
					size = (*it);
					std::cout << "size transmis pour le mode l dans chan" << size << std::endl;
					channel->setModes(channel->getModes() + "+l");

					// std::stringstream sstream(size);
					// size_t result = std::stoi(size);
					// sstream >> result;
					try
					{
						size_t result;
						std::stringstream sstream(size);
						sstream >> result;
						std::cout << RED << "size en int pour l" << result << RESET << std::endl;
						channel->setMaxclients(result);
						//channel->setModes(channel->getModes()); // faut revoir ça
						//channel->setParam(channel->getParam() + size);
					}
					catch(const std::exception& e)
					{
						std::cout << RED << "mauvais argument, pas un int " << RESET << std::endl;
						std::cerr << e.what() << '\n';
					}
					break;
				}
				
			}
			else 
			{
				client->reply(ERR_UNKNOWNMODE(client->getNickname(), (*it)));
			}
		}

	}
	channel->sendall(RPL_MODE(client->getPrefix(), channel->getName(), channel->getModes(), size));
	std::cout << GREEN << "mode du channel à la fin: " << channel->getModes() << RESET << std::endl;
}


void ModeCommand::execute(Client *client, std::vector<std::string> arg)
{
	// La commande MODE doit toujours avoir au moins un paramètre !
	if (arg.size() < 1)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		return;
	}
	// on aura deux types de paramètres : 
	//		- soit un nom de client		-> USER MODE
	//		- soit un nom de channel	-> CHANNEL MODE
	// on checke le paramètre : client ou channel ?
	std::string target = arg.at(0);

	Client *targetclient = _server->getClient(target);
	Channel *targetchannel = _server->getChannel(target);
	if (targetclient)
	{	
			mode_user(client, arg);
			if (client->getModes().find("o") != std::string::npos)
			{
				client->setOperator(1);
				std::cout << "operator oui non ?" << client->getOperator() << std::endl;
			}
	}
	else if (targetchannel)
	 		mode_channel(client, targetchannel, arg);	
	else
	{
		if (!targetchannel)
			client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), target));
		else
			client->reply(ERR_NOSUCHNICK(client->getNickname(), target));
	}
	std::cout << YELLOW << "Fin Mode command, modes du client: " << client->getModes() << RESET << std::endl;
}

/*
 Command: MODE
   Parameters: <nickname>
               *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )

   The user MODE's are typically changes which affect either how the
   client is seen by others or what 'extra' messages the client is sent.

   A user MODE command MUST only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.  If
   no other parameter is given, then the server will return the current
   settings for the nick.

      The available modes are as follows:

           a - user is flagged as away;
           i - marks a users as invisible;
           w - user receives wallops;
           r - restricted user connection;
           o - operator flag;
           O - local operator flag;
           s - marks a user for receipt of server notices.

   Additional modes may be available later on.

Kalt                         Informational                     [Page 12]
RFC 2812          Internet Relay Chat: Client Protocol        April 2000

   The flag 'a' SHALL NOT be toggled by the user using the MODE command,
   instead use of the AWAY command is REQUIRED.

   If a user attempts to make themselves an operator using the "+o" or
   "+O" flag, the attempt SHOULD be ignored as users could bypass the
   authentication mechanisms of the OPER command.  There is no
   restriction, however, on anyone `deopping' themselves (using "-o" or
   "-O").

   On the other hand, if a user attempts to make themselves unrestricted
   using the "-r" flag, the attempt SHOULD be ignored.  There is no
   restriction, however, on anyone `deopping' themselves (using "+r").
   This flag is typically set by the server upon connection for
   administrative reasons.  While the restrictions imposed are left up
   to the implementation, it is typical that a restricted user not be
   allowed to change nicknames, nor make use of the channel operator
   status on channels.

   The flag 's' is obsolete but MAY still be used.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_USERSDONTMATCH
           ERR_UMODEUNKNOWNFLAG            RPL_UMODEIS

   Examples:

   MODE WiZ -w                     ; Command by WiZ to turn off
                                   reception of WALLOPS messages.

   MODE Angel +i                   ; Command from Angel to make herself
                                   invisible.

   MODE WiZ -o                     ; WiZ 'deopping' (removing operator
                                   status).
*/