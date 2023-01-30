#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "server.hpp"
#include "client.hpp"

class Channel {

	public:
		Channel(const std::string &name);
		Channel(const std::string &name, const std::string &password, Client *admin);
		~Channel();

	private:

};

#endif