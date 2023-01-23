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
		throw std::runtime_error("Error while opening socket.");
		// 1.b/ définir les options du socket avec setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
	int val = 1; // non zero = enable a boolean option
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
		throw std::runtime_error("Error while setting socket options.");
		// 1.c/ mettre le socket en mode NON-BLOCKING (F_SETFL = set the file status flags to the value specified in 3rd argument)
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error while setting socket to NON-BLOCKING");

	// 2/ faire un lien avec le port
		// 2.a/ On va utiliser les variables de la structure générique sockaddr_in (sin) notamment : sin.addr et sin.port
	struct sockaddr_in serv_socket = {0}; // met toutes les variables à zero (évite certaines erreurs) et on recastera plus tard cette structure en sockaddr.
	serv_socket.sin_family = AF_INET;
	serv_socket.sin_addr.s_addr = htonl(INADDR_ANY); // notre serveur accepte n'importe quelle adresse
	serv_socket.sin_port = htons(std::stoi(_port)); // htons() converts unsigned short int to big-endian network byte order as expected from TCP protocol standards
		// 2.b/ on lie le socket avec l'adresse IP actuelle du port (bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen))
	if (bind(sockfd, (struct sockaddr *) &serv_socket, sizeof(serv_socket)) < 0)
		throw std::runtime_error("Error while binding socket.");
	
	// 3/ écouter et accepter une connexion entrante
		// 3.a/ écouter : listen()
	if (listen(sockfd, 1000) < 0) // 1000 = nb de connections simultanées acceptées par le serveur
		throw std::runtime_error("Error while listening on socket.");
		// 3.b/ accepter la connection d'un client : accept() renvoie un nouveau socket qu'on stocke dans une nvle struct !
	int connect_sockfd;
	sockaddr_in connect_serv_socket = {0}; // new structure pour une new connection ! (et mise à 0 de ses variables)
	socklen_t connectsize = sizeof connect_serv_socket;
	connect_sockfd = accept(sockfd, (sockaddr *) &connect_serv_socket, &connectsize);
	if (connect_sockfd < 0)
		throw std::runtime_error("Error while accepting a connexion.");
	return sockfd;
}
