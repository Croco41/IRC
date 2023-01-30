#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <cstring> // strstr
#include <stdio.h> // perror
#include <netdb.h> // getnameinfo
#include <cerrno> // errno
#include <stdlib.h> // strtol
#include <sys/socket.h> // socket
#include <sys/types.h> // bind
#include <arpa/inet.h> // htonl / htons / ntohl / ntohs
#include <stdexcept> // pour les exceptions throw !
#include <unistd.h> // fcntl
#include <fcntl.h> // fcntl
#include <iostream> // cerr
#include <sys/epoll.h> // epoll / epoll_create
#include <signal.h> // signal
#include <map> // map
#include <vector> // vector
#include "client.hpp"
#include "channel.hpp"

#define MAX_EVENTS 10

extern int	Run;

class Channel;

class Server
{
	public:
		Server(const std::string &port, const std::string &password);
		~Server(void);

		std::string	getPassword() const;
		int			getSocket() const;
		int			launch_socket();
		void		start_epoll();
		void		onClientConnect(sockaddr_in connect_serv_socket, int socket_client);
		void		onClientDisconnect(int fd, int epoll_fd);
		std::string	onClientMessage(int fd);
		//partie liée au channel:
		Channel*	getChannel(const std::string &name);
		Channel*	createChannel(const std::string &name, const std::string &password, Client *client);

	private:
		Server(Server const &copy);
		Server &operator=(Server const &rhs);

		const std::string		_port;
		const std::string		_password;
		int						_socket;
		std::map<int, Client *>	_clients; // nos clients seront enregistrés dans une map associant clé = fd, valeur = objet Client
		std::vector<Channel*>	_channels; // nos channels seront enregistrés dans un tableau
};

#endif