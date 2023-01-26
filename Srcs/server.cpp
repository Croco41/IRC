#include "../Includes/server.hpp"

Server::Server(const std::string &port, const std::string &password)
	: _port(port), _password(password)
{
	_socket = launch_socket();
}

Server::~Server(void)
{
	return;
}

std::string	Server::getPassword() const
{
	return (_password);
}

int			Server::getSocket() const
{
	return (_socket);
}

/*
LAUNCH_SOCKET : Cette fonction créée un socket attaché à un port
1/ initialiser le socket (socket() / setsockopt() / fcntl())
2/ faire un lien avec le port (bind())
3/ écouter et accepter une connexion entrante (listen() / accept())
4/ fermer le socket. (POUR PLUS TARD ATTENTION closesocket n'est pas une fonction autorisée... donc revenir à un close simple !)
Le protocole TCP est un protocole dit connecté. Il contrôle si le paquet est arrivé à destination, si ce n'est pas le cas il le renvoie.
*/
int			Server::launch_socket()
{
	// 1/ initialiser le socket
		// 1.a/ ouvrir un fd socket avec socket(int domain, int type, int protocol)
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET pour IPv4 Internet Protocol (si on veut IPv6 : AF_INET6) / SOCK_STREAM : full duplex-byte streams / 0 : un seul protocole supporté par le socket (ici IPv4)
	if (sockfd < 0)
		throw std::runtime_error("Error while opening socket.\n");
		// 1.b/ définir les options du socket avec setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
	int val = 1; // non zero = enable a boolean option
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
		throw std::runtime_error("Error while setting socket options.\n");
		// 1.c/ mettre le socket en mode NON-BLOCKING (F_SETFL = set the file status flags to the value specified in 3rd argument)
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.\n");

	// 2/ faire un lien avec le port
		// 2.a/ On va utiliser les variables de la structure générique sockaddr_in (sin) notamment : sin.addr et sin.port
	struct sockaddr_in serv_socket = {AF_INET, 0, {0}, {0}}; // met toutes les variables à zero (évite certaines erreurs) et on recastera plus tard cette structure en sockaddr.
	serv_socket.sin_family = AF_INET;
	serv_socket.sin_addr.s_addr = htonl(INADDR_ANY); // notre serveur accepte n'importe quelle adresse
	// serv_socket.sin_port = htons(std::strtol(_port.c_str(), NULL, 10)); // htons() converts unsigned short int to big-endian network byte order as expected from TCP protocol standards
	try
	{
		int port = std::strtol(_port.c_str(), NULL, 10);
		if (port < 0 || port > 65535)
			throw std::invalid_argument("Invalid port number.\n");
		serv_socket.sin_port = htons(port);
	}
	catch (const std::invalid_argument &e)
	{
		perror("Error: Invalid port number.\n");
		exit(EXIT_FAILURE);
	}
		// 2.b/ on lie le socket avec l'adresse IP actuelle du port (bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen))
	if (bind(sockfd, (struct sockaddr *) &serv_socket, sizeof(serv_socket)) < 0)
		throw std::runtime_error("Error while binding socket.\n");
	
	// 3/ écouter et accepter une connexion entrante
		// 3.a/ écouter : listen()
	if (listen(sockfd, 1000) < 0) // 1000 = nb de connections simultanées acceptées par le serveur
		throw std::runtime_error("Error while listening on socket.\n");
		// 3.b/ accepter la connection d'un client : accept() renvoie un nouveau socket qu'on stocke dans une nvle struct !
	std::cout << "On arrive ici !" << std::endl;
		// int connect_sockfd;
		// sockaddr_in connect_serv_socket = {AF_INET, 0, {0}, {0}};
		// socklen_t connectsize = sizeof connect_serv_socket;
	// while (true)
	// {
	// 	connect_sockfd = accept(sockfd, (sockaddr *) &connect_serv_socket, &connectsize);
	// 	if (connect_sockfd < 0)
	// 	{
	// 		if (errno == EINTR)
	// 			continue;
	// 		throw std::runtime_error("Error while accepting a connexion YOUHOU.");
	// 	}
	// }

	// int connect_sockfd;
	// sockaddr_in connect_serv_socket = {AF_INET, 0, {0}, {0}}; // new structure pour une new connection ! (et mise à 0 de ses variables)
	// socklen_t connectsize = sizeof connect_serv_socket;
	// connect_sockfd = accept(sockfd, (sockaddr *) &connect_serv_socket, &connectsize);
	// if (connect_sockfd < 0)
	// 	throw std::runtime_error("Error while accepting a connexion.");
	return sockfd;
}

void		Server::start_epoll()
{
	int epoll_fd = epoll_create1(0); // create a new epoll instance
	if (epoll_fd < 0)
		throw std::runtime_error("Error while creating epoll file descriptor.\n");

	struct epoll_event server_ev;
	server_ev.data.fd = _socket;
	server_ev.events = EPOLLIN; // monitor for incoming data. EPOLLIN : The associated file is available for read(2) operations.

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _socket, &server_ev))
		throw std::runtime_error("Error while adding server socket to epoll.\n");
	std::cout << "Server listening..." << std::endl;
	// ft_log("Server listening...");
	struct epoll_event connect_ev[MAX_EVENTS];
	while (1)
	{
		// std::cout << "Polling for input..." << std::endl;
		int nfds = epoll_wait(epoll_fd, connect_ev, MAX_EVENTS, -1); // -1 = infinite timeout
		if (nfds < 0)
			throw std::runtime_error("Error while waiting for epoll.\n");
		// std::cout << nfds << " ready events." << std::endl;
		for (int i = 0; i < nfds; i++)
		{
			int fd = connect_ev[i].data.fd;
//std::cout << "fd" << fd << "   /notre socket server: " << _socket << std::endl;
			if (fd == _socket)
			{
				int connect_sockfd;
				sockaddr_in connect_serv_socket = {AF_INET, 0, {0}, {0}};
				socklen_t connectsize = sizeof connect_serv_socket;
				connect_sockfd = accept(_socket, (struct sockaddr *) &connect_serv_socket, &connectsize);
//std::cout << "fd" << fd << "   /connect_sockfd: " << connect_sockfd << std::endl;
				if (connect_sockfd < 0)
				{
					if (errno == EINTR)
						continue;
					throw std::runtime_error("Error while accepting a connexion YOUHOU.\n");
				}
				server_ev.data.fd = connect_sockfd;
//std::cout << "fd" << fd << "   /server_ev.data.fd: " << server_ev.data.fd << std::endl;
				server_ev.events = EPOLLIN;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connect_sockfd, &server_ev) == 0)
				{
					std::cout << "Client n°" << server_ev.data.fd << " s'est connecté !" << std::endl;
					onClientConnect(connect_serv_socket, connect_sockfd);
					//continue;
				}
//std::cout << " ????? " << std::endl;
				//onClientMessage(fd);
				//std::cout << " ????? " << std::endl;
			}
			else
			{
//std::cout << "On arrive la" << std::endl;
				char buffer[1024];
				ssize_t n = recv(fd, buffer, sizeof buffer, 0);
				if(n == -1)
				{
					perror("Error while receiving data.");
					break;
				}
				else if(n == 0)
				{
					onClientDisconnect(fd, epoll_fd);
					continue;
				}
				// else
				// {
				// 	onClientMessage(fd);
				// 	continue;
				// }
			}
		}
	}
	if (close(epoll_fd))
		throw std::runtime_error("Error while closing epoll file descriptor.\n");
}

void		Server::onClientConnect(sockaddr_in connect_serv_socket, int socket_client)
{
	char hostname[100];
	if (getnameinfo((struct sockaddr *) &connect_serv_socket, sizeof(connect_serv_socket), hostname, 100, NULL, 0, NI_NUMERICSERV) != 0)
		throw std::runtime_error("Error while getting hostname on new client.");
	//client à faire??

	Client *client = new Client(hostname, socket_client, ntohs(connect_serv_socket.sin_port));
	std::cout << hostname << ":" << ntohs(connect_serv_socket.sin_port) << " has connected." << std::endl << std::endl;
	std::cout << connect_serv_socket.sin_addr << std::endl << std::endl;


	std::string message;
	char tmp[100] = {0};
	while (message.find("\r\n") == std::string::npos)
	{
		int r = recv(socket_client, tmp, 100, 0);
		if (r < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("Error while receiving message from client.");			
		}
		message.append(tmp, r);
	}
	std::cout << "message = " << message << std::endl;
	std::cout << client->getHostname() << std::endl;
	client->setRealname("test");
	std::cout << client->getRealname() << std::endl;
}

void		Server::onClientDisconnect(int fd, int epoll_fd)
{
	// Remove the client from the epoll instance (flag EPOLL_CTL_DEL)
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, 0);

	// Close the socket for this client
	close(fd);

	// Log the client disconnection
	std::cout << "Client n°" << fd << " s'est déconnecté." << std::endl;
}

std::string	Server::onClientMessage(int fd)
{
	std::string message;
	char tmp[100] = {0};
	while (message.find("\r\n") == std::string::npos)
	{

		int r = recv(fd, tmp, 100, 0);
		if (r < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("Error while receiving message from client.");			
		}
		message.append(tmp, r);
	}
	std::cout << "message = " << message << std::endl;
	return (message);
}
