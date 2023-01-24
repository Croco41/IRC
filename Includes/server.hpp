#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <stdio.h> // perror
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

#define MAX_EVENTS 10

class Server
{
	public:
		Server(const std::string &port, const std::string &password);
		~Server(void);

		std::string	getPassword() const;
		int			getSocket() const;
		int			launch_socket();
		void		start_epoll();

	private:
		Server(Server const &copy);
		Server &operator=(Server const &rhs);

		const std::string	_port;
		const std::string	_password;
		int					_socket;
};

#endif