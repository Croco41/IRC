#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client;

#include <string>
#include <sstream>
#include <sys/poll.h>
#include <sys/socket.h>

#include "channel.hpp"
#include "utils.hpp"


class Client
{
	public:
		Client(const std::string &hostname, int fd, int port);
		~Client();

		// GETTERS
		std::string		getHostname() const;
		int				getFd() const;
		int				getPort() const;
		std::string		getRealname() const;
		std::string		getUsername() const;
		std::string		getNickname() const;
		std::string		getPassword() const;
		Channel			*getChannel() const;

		// SETTERS
		void			setRealname(const std::string &realname);
		void			setUsername(const std::string &username);
		void			setNickname(const std::string &nickname);
		void			setPassword(const std::string &password);
		void			setChannel(Channel *channel);

		// FONCTIONS MEMBRES
		void			reply(const std::string &reply);
		void 			writetosend(const std::string &message) const;
		void			join_channel(Channel *channel);

	private:
		// variables initialisées lors de la construction
		std::string		_hostname;
		int				_fd;
		int				_port;
		// variables initialisées via setters
		std::string		_realname;
		std::string		_username;
		std::string		_nickname;
		std::string		_password;
		Channel			*_channel;

		//fonctions pour les messages
		std::string		getPrefix() const;	
};

#endif