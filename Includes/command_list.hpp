#ifndef COMMAND_LIST_HPP
# define COMMAND_LIST_HPP

class Command;

#include "colors.hpp"
#include "server.hpp"
#include "client.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <algorithm>

#define NICK_VALID_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-^_[]{}\\|"
#define MODES_USER "aiwro"
// t
//type A modes are lists that can be viewed:
// When the channel flag 'i' is set, new members are only accepted if
  // their mask matches Invite-list (See section 4.3.2) or they have been
   //invited by a channel operator.
//Invite-Exception List "+I": Invite-exception lists are returned with zero or
// more RPL_INVITELIST (336) numerics, followed by one RPL_ENDOFINVITELIST (337) numeric.
// l => Type C: Modes that change a setting on a channel. These modes MUST have a parameter when being set
//, and MUST NOT have a parameter when being unset.
#define CHANNEL_MODES "tlo" //ki//
// CHANNEL MODES :
// for users :
//     - o : channel operator
// for channels :
//     - i : invite only
//     - t : topic locked
//     - l : user limit
// USER MODES :
// - a : away
// - i : invisible
// - w : wallops
// - r : restricted
// - o : operator


// Command est ici une classe interface qui donne le patron de chaque commande

class Command
{
	public:
		Command(Server *server) : _server(server){};
		virtual ~Command(){};

		virtual void execute(Client *client, std::vector<std::string> arg) = 0;

	protected:
		Server	*_server;
};

// ici on aura toutes les classes dérivées de Command : join, quit, user, nick...

class UserCommand : public Command
{
	public:
		UserCommand(Server *server);
		~UserCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class NickCommand : public Command
{
	public:
		NickCommand(Server *server);
		~NickCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class PingCommand : public Command
{
	public:
		PingCommand(Server *server);
		~PingCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class PongCommand : public Command
{
	public:
		PongCommand(Server *server);
		~PongCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class WhoCommand : public Command
{
	public:
		WhoCommand(Server *server);
		~WhoCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class JoinCommand : public Command
{
	public:
		JoinCommand(Server *server);
		~JoinCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class PartCommand : public Command
{
	public:
		PartCommand(Server *server);
		~PartCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class PrivMsgCommand : public Command
{
	public:
		PrivMsgCommand(Server *server);
		~PrivMsgCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class NoticeCommand : public Command
{
	public:
		NoticeCommand(Server *server);
		~NoticeCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

class ModeCommand : public Command
{
	public:
		ModeCommand(Server *server);
		~ModeCommand();

		void execute(Client *client, std::vector<std::string> arg);
		void mode_user(Client *client, std::vector<std::string> arg);
		void mode_channel(Client *client, Channel *channel, std::vector<std::string> arg);
};

class QuitCommand : public Command
{
	public:
		QuitCommand(Server *server);
		~QuitCommand();
	
		void execute(Client *client, std::vector<std::string> arg);
};

class OperCommand : public Command
{
	public:
		OperCommand(Server *server);
		~OperCommand();

		void execute(Client *client, std::vector<std::string> arg);
};

#endif