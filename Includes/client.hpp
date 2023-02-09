#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client;

#include <string>
#include <sstream>
#include <sys/poll.h>
#include <sys/socket.h>

#include "colors.hpp"
#include "channel.hpp"
#include "utils.hpp"


class Client
{
	public:
		Client(const std::string &hostname, int fd, int port);
		~Client();

		// GETTERS
		std::string				getHostname() const;
		int						getFd() const;
		int						getPort() const;
		std::string				getRealname() const;
		std::string				getUsername() const;
		std::string				getNickname() const;
		std::string				getPassword() const;
		bool				getRegistered() const;
		std::vector<Channel *>	getChannel() const;

		//fonctions pour les messages
		std::string		getPrefix() const;

		// SETTERS
		void			setRealname(const std::string &realname);
		void			setUsername(const std::string &username);
		void			setNickname(const std::string &nickname);
		void			setPassword(const std::string &password);
		void			setRegistered(const bool &isregistered);
		void			setChannel(Channel *channel);

		// FONCTIONS MEMBRES
		bool			findChannel(std::string chan_name);
		void			reply(const std::string &reply);
		void			reply_command(const std::string &reply);
		void 			writetosend(const std::string &message) const;
		void			join_channel(Channel *channel);
		void			leave_channel(Channel *channel, std::string message);

	private:
		// variables initialisées lors de la construction
		std::string				_hostname;
		int						_fd;
		int						_port;
		// variables initialisées via setters
		std::string				_realname;
		std::string				_username;
		std::string				_nickname;
		std::string				_password;
		bool				_isregistered;
		// Channel					*_channel; // il faut un vecteur de channel !!
		std::vector<Channel *>	_Cchannels; // le client peut se connecter à pls channels !
};

#endif