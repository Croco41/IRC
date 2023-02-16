#include "../Includes/server.hpp"

int Run;

Server::Server(const std::string &port, const std::string &password)
	: _port(port), _password(password), _servname("ircserv"), _channels(), _errorpass(0)  
{
	Run = 1;
	_socket = launch_socket();
	_commandHandler = new CommandHandler(this); 
}

Server::~Server(void)
{
	std::cout << RED << "SERVER : Destructor !" << RESET << std::endl;
	// ---------- delete Clients ---------- //
	std::cout << INDIANRED << "nos clients à deleter : " << RESET << std::endl;
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		std::cout << "	client " << it->second->getNickname() << std::endl;

	if (_clients.size())
	{
		std::map<int, Client *>::iterator it_cl;
		for (it_cl= _clients.begin(); it_cl != _clients.end(); ++it_cl)
		{
			close(it_cl->first);
			delete it_cl->second;
		}
		_clients.clear();
	}
	// ---------- delete Channels ---------- //
	std::cout << INDIANRED << "nos channels à deleter : " << RESET << std::endl;
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		std::cout << "	channel " << it.operator*()->getName() << std::endl;

	if (_channels.size())
	{
		std::vector<Channel *>::iterator it;
		for (it = _channels.begin(); it != _channels.end(); ++it)
			delete *it;
		_channels.clear();
	}
	delete _commandHandler;
	close(_socket);
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

std::string	Server::getServname() const
{
	return (_servname);
}

int			Server::getEpollfd() const
{
	return (_epollfd);
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

Client*		Server::getClient(const std::string &nickname)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (!nickname.compare(it->second->getNickname()))
			return (it->second);
	}
	return NULL;
}

bool			Server::getErrorPass()
{
	return(this->_errorpass);
}

void			Server::setErrorPass(bool errorpass)
{
	_errorpass = errorpass;
}

std::map<int, Client *>	Server::getClients() const
{
	return (_clients);
}

std::vector<Channel *>	Server::getChannels() const
{
	return (_channels);
}

void		handleSignal(int sigint)
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
	int epoll_fd = epoll_create1(0); // create an epoll instance
	_epollfd = epoll_fd;
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
				sockaddr_in connect_serv_socket = {AF_INET, 0, {0}, {0}}; // nouvelle structure pour une nouvelle connection ! (et mise à 0 de ses variables)
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
					break;
					//continue;
				}
			}
			else
			{
				char buffer[1024];
				bzero(buffer, 1024);
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
					onClientMessage(fd, buffer, n);
					//break;
				}
			}
			consolDeBUGserver();
		}
	}
	if (close(epoll_fd))
		throw std::runtime_error("Error while closing epoll file descriptor.\n");
}

std::string	Server::ParsingonClientConnect(std::string message, std::string word, Client *client)
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
	if (word == "NICK")
	{
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			if (newword == it.operator*().second->getNickname())
				newword.push_back('_');
		}
	}
	return(newword);
}

std::string	Server::recvMessage(int socket_client, char *tmp, size_t r)
{
	std::string message;
	message.append(tmp, r);
	while ((message.find("\r\n") == std::string::npos) || (message.find("\n") == std::string::npos))
	{
		bzero(tmp, 100);
		int r = recv(socket_client, tmp, 1024, 0);
		if (r < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("Error while receiving message from client.");			
		}
		message.append(tmp, r);
	}
	return (message);	
}

void		Server::onClientConnect(sockaddr_in connect_serv_socket, int socket_client)
{
	char hostname[100];
	if (getnameinfo((struct sockaddr *) &connect_serv_socket, sizeof(connect_serv_socket), hostname, 100, NULL, 0, NI_NUMERICSERV) != 0)
		throw std::runtime_error("Error while getting hostname on new client.");

	Client *client = new Client(hostname, socket_client, ntohs(connect_serv_socket.sin_port));
	std::cout << hostname << ":" << ntohs(connect_serv_socket.sin_port) << " has connected." << std::endl << std::endl;
	_clients.insert(std::make_pair(socket_client, client)); // on enregistre l'instance client au niveau de la clé fd (il ne peut y avoir 2x le même fd, donc map parfait !)

	//std::string message;
	std:: string message;
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
	// client->setPassword(ParsingonClientConnect(message, "PASS", client));
	// client->setNickname(ParsingonClientConnect(message, "NICK", client));
	// client->setRealname(ParsingonClientConnect(message, "USER", client));
	
	// std::cout << "password: " << client->getPassword() << std::endl;
	// std::cout << "nickname: " << client->getNickname() << std::endl;
	// std::cout << "username: " << client->getUsername() << std::endl;
	// std::cout << "realname: " << client->getRealname() << std::endl;

	_commandHandler->recup_msg(client, message);
	std::cout << YELLOW;
	if (_errorpass == 0)
	{
		client->setRegistered(1);
		client->reply(RPL_WELCOME(client->getNickname()));
		std::cout << "password: " << client->getPassword() << std::endl;
		std::cout << "nickname: " << client->getNickname() << std::endl;
		std::cout << "username: " << client->getUsername() << std::endl;
		std::cout << "realname: " << client->getRealname() << std::endl;
		std::cout << RESET;
	}
	else
	{
		_errorpass = 0;
		client->reply_command(RPL_ERROR(client->getNickname()));
		onClientDisconnect(client->getFd(), getEpollfd());
	}

	// createChannel("Fantasy", "Gandalf", client);
}

void		Server::onClientDisconnect(int fd, int epoll_fd)
{
	// Remove the client from the epoll instance (flag EPOLL_CTL_DEL)
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, 0);
	// Close the socket for this client
	std::cout << "Client n°" << fd << " va se déconnecter." << std::endl;
	delete _clients.at(fd);
	_clients.erase(fd);
	close(fd);
	// Log the client disconnection
	std::cout << "Client n°" << fd << " s'est déconnecté." << std::endl;
}

void	Server::onClientMessage(int fd, char *tmp, size_t r)
{
	try
	{
		std::string message = recvMessage(fd, tmp, r);
		std::cout << RED << "\nmessage = " << message << RESET;
		Client	*client = _clients.at(fd);
		_commandHandler->recup_msg(client, message);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

Channel*	Server::createChannel(const std::string &name, const std::string &password, Client *client)
{
	std::cout << PURPLE << "\nSERVER : createChannel - start" << RESET << std::endl;
	Channel *channel = new Channel(name, password, client);
	client->setChannel(channel);
	// std::cout << "Nom du channel auquel est lié le client :" << client->getChannel(channel)->getName() << std::endl;
	_channels.push_back(channel);
	std::cout << "channel name = " << _channels.front()->getName() << std::endl;
	std::cout << "password = ";
	if (_channels.front()->getPassword() == "")
		std::cout << "no password registered" << std::endl;
	else
		std::cout << _channels.front()->getPassword() << std::endl;
	std::cout << "client nickname = " << _channels.front()->getAdmin()->getNickname() << std::endl;
	std::cout << PURPLE << "SERVER : createChannel - end" << RESET << std::endl;
	return (channel);
}

void	Server::destroyChannel(Channel *channel)
{
	std::cout << YELLOW << "\nSERVER : destroyChannel - start" << RESET << std::endl;
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		std::cout << it.operator*()->getName() << std::endl;
		std::cout << ROYALBLUE << "taille du vector de channels dans server : " << _channels.size() << RESET << std::endl;

		if (*it == channel)
		{
			// delete (*it);
			// _channels.erase(std::remove(_channels.begin(), _channels.end(), channel), _channels.end());
			delete (*it);
			_channels.erase(it);
			break;
		}
	}
	std::cout << YELLOW << "SERVER : destroyChannel - end" << RESET << std::endl;
}

void Server::consolDeBUGserver()
{
	std::cout << RED << "-----------------START CONSOL DEBUG SERVER--------------" << RESET << std::endl << std::endl;

//-------------------------------------------------------------------------------------------------
	std::cout << YELLOW << "-----------------CHECK CHANNELS--------------" << RESET << std::endl;

	std::string list_channels;
	
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		Channel *channel = it.operator*();
		std::string name = channel->getName(); 
		list_channels.append(name);
		channel->consolDeBUGchannel();
	}
	std::cout << YELLOW << "list_channels: " << RESET << std::endl;
	std::cout << GREEN << list_channels << RESET << std::endl;


//-------------------------------------------------------------------------------------------------
	std::cout << YELLOW << "-----------------CHECK CLIENTS--------------" << RESET << std::endl;

	std::cout << YELLOW << "liste des clients avec leurs fds associes: " << RESET << std::endl;
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		Client *client = it->second;
		std::string name = client->getNickname(); 
		int fd = it->first;
		std::cout << GREEN << "-->Nickname: " << name << "-->fd: "<< fd << RESET << std::endl;
	}
}
