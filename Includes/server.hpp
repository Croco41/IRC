#ifndef SERVER_HPP
# define SERVER_HPP

class Server; // on déclare la class Server avant de la définir complètement, parce qu'il y a des aller-retour entre server, client et channel... 

#include <stdio.h> // perror
#include <netdb.h> // getnameinfo
#include <stdlib.h> // strtol
#include <sys/socket.h> // socket
#include <sys/types.h> // bind
#include <arpa/inet.h> // htonl / htons / ntohl / ntohs
#include <unistd.h> // fcntl
#include <fcntl.h> // fcntl
#include <sys/epoll.h> // epoll / epoll_create
#include <signal.h> // signal

#include <cerrno> // errno
#include <stdexcept> // pour les exceptions throw !
#include <iostream> // cerr
#include <string>
#include <cstring> // strstr
#include <map> // map
#include <vector> // vector

#include "colors.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "commandhandler.hpp"
#include "utils.hpp"

#define MAX_EVENTS 10

extern int	Run;

// class Channel; hello ISA

class Server
{
	typedef std::map<int, Client *>::iterator it_cl;
	typedef std::vector<Channel *>::iterator it;

	public:
		Server(const std::string &port, const std::string &password);
		~Server(void);

		std::string				getPassword() const;
		std::map<int, Client *>	getClients() const;
		int						getSocket() const;
		int						launch_socket();
		void					start_epoll();
		void					onClientConnect(sockaddr_in connect_serv_socket, int socket_client);
		std::string				ParsingonClientConnect(std::string message, std::string word, Client *client);
		void					onClientDisconnect(int fd, int epoll_fd);
		void					onClientMessage(int fd, char *tmp, size_t r);
		std::string				recvMessage(int socket_client, char *tmp, size_t r);
		//partie liée au channel:
		Channel*				getChannel(const std::string &name);
		Channel*				createChannel(const std::string &name, const std::string &password, Client *client);
		void					destroyChannel(Channel *channel);
		//pour avoir accès au client
		Client*					getClient(const std::string &nickname);
		std::string				getServname() const;


	private:
		Server(Server const &copy);
		Server &operator=(Server const &rhs);

		const std::string		_port;
		const std::string		_password;
		const std::string		_servname;
		int						_socket;
		std::map<int, Client *>	_clients; // nos clients seront enregistrés dans une map associant clé = fd, valeur = objet Client
		std::vector<Channel *>	_channels; // nos channels seront enregistrés dans un tableau
		CommandHandler			*_commandHandler;
};

#endif