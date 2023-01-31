#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Channel;

#include "server.hpp"
#include "client.hpp"

class Channel
{
	public:
		Channel(const std::string &name, const std::string &password);
		Channel(const std::string &name, const std::string &password, Client *admin);
		~Channel();

		// GETTERS
		std::string		getName() const;
		std::string		getPassword() const;
		Client	 		*getAdmin();
		size_t			getMaxclients() const;
		size_t			getNbclients() const;

		// SETTERS
		void			setPassword(const std::string password);
		void			setMaxclients(const size_t maxclients);
		void			setNbclients(const size_t nbclients);

	private:
		std::string				_name;
		std::string				_password;
		Client					*_admin;
		std::vector<Client *>	_clients;
		size_t					_maxclients;
		size_t					_nbclients;
};

#endif