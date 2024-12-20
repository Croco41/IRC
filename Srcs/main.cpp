#include "server.hpp"

int main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
			throw std::runtime_error("Usage: ./ircserv <port> <password>");
		Server server(argv[1], argv[2]);
		server.start_epoll();
		return 0;
	}
	catch (const std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
}