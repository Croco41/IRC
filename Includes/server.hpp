#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <sys/socket.h> // socket
#include <sys/types.h> // bind
#include <arpa/inet.h> // htonl / htons / ntohl / ntohs
#include <stdexcept> // pour les exceptions throw !
#include <unistd.h> // fcntl
#include <fcntl.h> // fcntl

class Server
{
	public:
		Server(const std::string &port, const std::string &password);
		~Server(void);

		std::string	getPassword() const;
		int			getSocket() const;
		int			launch_socket();

	private:
		Server(Server const &copy);
		Server &operator=(Server const &rhs);

		const std::string   _password;
		const std::string   _port;
		int					_socket;
};

#endif