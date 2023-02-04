#include "../Includes/server.hpp"

int Run;

Server::Server(const std::string &port, const std::string &password)
	: _port(port), _password(password)
{
	Run = 1;
	_socket = launch_socket();
	_commandHandler = new CommandHandler(this);
}

Server::~Server(void)
{
	std::cout << "on passe par ici" << std::endl;
    if (_clients.size())
    {
        typename std::map<int, Client *>::iterator it;
        for (it = _clients.begin(); it != _clients.end(); ++it)
        {
            close(it->first);
            delete it->second;
        }
        _clients.clear();
    }
	delete _commandHandler;
	std::cout << "Server closed." <<std::endl;
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

Channel*	Server::getChannel(const std::string &name)
{
	typedef std::vector<Channel *>::iterator chan_iterator;
	for (chan_iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it.operator*()->getName() == name)
			return (it.operator*());
	}
	return NULL;
}

void handleSignal(int sigint)
{
	std::cout << std::endl;
	std::cout << "Exiting server..." << std::endl;
	if (sigint == SIGINT)
		Run = 0;
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
	signal(SIGINT, handleSignal);
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
	try
	{
		int port = std::strtol(_port.c_str(), NULL, 10);
		if (port < 0 || port > 65535)
			throw std::invalid_argument("Invalid port number.\n");
		serv_socket.sin_port = htons(port); // htons() converts unsigned short int to big-endian network byte order as expected from TCP protocol standards
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
	return sockfd;
}

void		Server::start_epoll()
{
	int epoll_fd = epoll_create1(0); // create a new epoll instance
	if (epoll_fd < 0)
		throw std::runtime_error("Error while creating epoll file descriptor.\n");
	// if (epoll_fd == -1) 
	// {
    //     perror("epoll_create1");
    //     exit(EXIT_FAILURE);
    // }
	struct epoll_event server_ev;
	memset(&server_ev, 0, sizeof(server_ev)); // Initialiser tous les octets de event à zéro
	server_ev.data.fd = _socket;
	server_ev.events = EPOLLIN; // monitor for incoming data. EPOLLIN : The associated file is available for read(2) operations.
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _socket, &server_ev))
		throw std::runtime_error("Error while adding server socket to epoll.\n");
	struct epoll_event connect_ev[MAX_EVENTS];
	std::cout << "Server listening..." << std::endl;
	// ft_log("Server listening...");
	while (Run)
	{
		if (Run == 0)
			break;
		int nfds = epoll_wait(epoll_fd, connect_ev, MAX_EVENTS, -1); // -1 = infinite timeout
		if (nfds < 0)
		{
			if (Run == 0)
				break;
			throw std::runtime_error("Error while waiting for epoll.\n");
		}
		for (int i = 0; i < nfds; i++)
		{
			int fd = connect_ev[i].data.fd;
			if (fd == _socket)
			{
				int connect_sockfd;
				sockaddr_in connect_serv_socket = {AF_INET, 0, {0}, {0}}; // new structure pour une new connection ! (et mise à 0 de ses variables)
				socklen_t connectsize = sizeof connect_serv_socket;
				connect_sockfd = accept(_socket, (struct sockaddr *) &connect_serv_socket, &connectsize); //accepter la connection d'un client : accept() renvoie un nouveau socket qu'on stocke dans une nvle struct !
				if (connect_sockfd < 0)
				{
					if (errno == EINTR)
						continue;
					throw std::runtime_error("Error while accepting a connexion YOUHOU.\n");
				}
				server_ev.data.fd = connect_sockfd;
				server_ev.events = EPOLLIN;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connect_sockfd, &server_ev) == 0)
				{
					std::cout << "Client n°" << server_ev.data.fd << " s'est connecté !" << std::endl;
					onClientConnect(connect_serv_socket, connect_sockfd);
					
					//continue;
				}
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
				else
				{
					onClientMessage(fd);
					std::cout << "quit ClientMsg ?" << std::endl;
					continue;
				}
			}
		}
	}
	if (close(epoll_fd))
		throw std::runtime_error("Error while closing epoll file descriptor.\n");
}

std::string	ParsingonClientConnect(std::string message, std::string word, Client *client)
{
		std::string newword;
		size_t found = message.find(word);
		size_t start;
		size_t end;

		if (word == "USER")
		{
			if (found != std::string::npos)
			{
				start = message.find_first_not_of("\t\n", found + word.length() + 1);
				end = message.find_first_of(" ", start);
				newword = message.substr(start, end - start);
				client->setUsername(newword);
				start = message.find(":");
				end = message.find_first_of("\t\n", start);
				newword = message.substr(start + 1, end - start -1);
			}
		}
		else if (found != std::string::npos)
		{
			start = message.find_first_not_of("\t\n", found + word.length() + 1);
			end = message.find_first_of("\t\n", start);
			newword = message.substr(start, end - start -1);
		}
		else
			newword = "";
		return(newword);
}

void		Server::onClientConnect(sockaddr_in connect_serv_socket, int socket_client)
{
	char hostname[100];
	if (getnameinfo((struct sockaddr *) &connect_serv_socket, sizeof(connect_serv_socket), hostname, 100, NULL, 0, NI_NUMERICSERV) != 0)
		throw std::runtime_error("Error while getting hostname on new client.");

	Client *client = new Client(hostname, socket_client, ntohs(connect_serv_socket.sin_port));
	std::cout << hostname << ":" << ntohs(connect_serv_socket.sin_port) << " has connected." << std::endl << std::endl;
	_clients.insert(std::make_pair(socket_client, client)); // on enregistre l'instance client au niveau de la clé fd (il ne peut y avoir 2x le même fd, donc map parfait !)

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
	client->setPassword(ParsingonClientConnect(message, "PASS", client));
	client->setNickname(ParsingonClientConnect(message, "NICK", client));
	client->setRealname(ParsingonClientConnect(message, "USER", client));
	
	std::cout << "password: " << client->getPassword() << std::endl;
	std::cout << "nickname: " << client->getNickname() << std::endl;
	std::cout << "username: " << client->getUsername() << std::endl;
	std::cout << "realname: " << client->getRealname() << std::endl;

	std::cout << YELLOW;
	client->reply(RPL_WELCOME(client->getNickname()));
	std::cout << RESET;

	// createChannel("Fantasy", "Gandalf", client);
}

void		Server::onClientDisconnect(int fd, int epoll_fd)
{
	// Remove the client from the epoll instance (flag EPOLL_CTL_DEL)
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, 0);
	// Close the socket for this client
	std::cout << "Client n°" << fd << " username: " << _clients.at(fd)->getUsername() << " va se déconnecter." << std::endl;
	delete _clients.at(fd);
	_clients.erase(fd);
	close(fd);
	// Log the client disconnection
	std::cout << "Client n°" << fd << " s'est déconnecté." << std::endl;
}

// std::string	Server::onClientMessage(int fd)
void	Server::onClientMessage(int fd)
{
	std::string message;
	char tmp[100] = {0};
std::cout << GREEN << "ON ENTRE Dans Server::OnClientMessage !" << RESET << std::endl;
	while (message.find("\r\n") == std::string::npos)
	{
		std::cout << LIMEGREEN << "boucle while, ici tmp = " << tmp << RESET << std::endl;
		int r = recv(fd, tmp, 100, 0);
		std::cout << FORESTGREEN << "after recv" << RESET << std::endl;
		if (r < 0)
		{
			std::cout << "dans r < 0 on Server::Clientmessage" << std::endl; 
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("Error while receiving message from client.");			
		}
		std::cout << r << std::endl;
		message.append(tmp, r);
	}
	std::cout << FUCHSIA << "message = " << message << RESET << std::endl;
	try
	{
		Client	*client = _clients.at(fd);
		std::cout << "on est dans le try-catch de onClientMessage !" << std::endl;
		_commandHandler->recup_msg(client, message);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return;
}

// void    ParsingonClientConnect(std::string message, Client *client)
// {

// 	std::cout << "dans la boucle message principale" <<std::endl;
// 	size_t pos = message.find("PASS") + 5;
// 	std::cout << pos << std::endl;

// 	if(pos <= message.length())
// 	{
// 		std::cout << "dans la boucle message.at" <<std::endl;
// 		std::cout << pos << message[pos] << std::endl;

// 		std::string content;
// 		int end = pos;
// 		while(message[end] != '\n')
// 		{
// 			end++;
// 		}
// 		client->setPassword(content.append(message, pos, end));

// 		// while(!message.find("\n"))
// 		// {
// 		//     std::cout << "dans la boucle setpassword" <<std::endl;
// 		//     client->setPassword(message);
// 		// }
//         }
// }

Channel*	Server::createChannel(const std::string &name, const std::string &password, Client *client)
{
	// if (client == NULL)
	// 	Channel *channel = new Channel(name, password);
	// else
	Channel *channel = new Channel(name, password, client);
	_channels.push_back(channel);
	std::cout << "creation de notre premier channel !" << std::endl;
	std::cout << "name of the channel = " << _channels.front()->getName() << std::endl;
	std::cout << "password = " << _channels.front()->getPassword() << std::endl;
	std::cout << "nom du Client = " << _channels.front()->getAdmin()->getNickname() << std::endl;
	return (channel);
}
