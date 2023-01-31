#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include <sstream>

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

		// SETTERS
		void			setRealname(const std::string &realname);
		void			setUsername(const std::string &username);
		void			setNickname(const std::string &nickname);
		void			setPassword(const std::string &password);

	private:
		// variables initialisées lors de la construction
		std::string		_hostname;
		int				_fd;
		int				_port;
		// variables initialisées 
		std::string		_realname;
		std::string		_username;
		std::string		_nickname;
		std::string		_password;
};

#endif