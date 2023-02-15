#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Channel;

#include "colors.hpp"
#include "server.hpp"
#include "client.hpp"

class Channel
{
	public:
		Channel(const std::string &name, const std::string &password);
		Channel(const std::string &name, const std::string &password, Client *admin);
		~Channel();

		// GETTERS
		std::string					getName() const;
		std::string					getPassword() const;
		Client	 					*getAdmin();
		size_t						getMaxclients() const;
		size_t						getNbclients() const;
		size_t						getNboperators() const; //ok
		std::vector<Client *>		getOperators() const;
		std::string					getNicknamesList();
		std::vector<std::string>	getNicknames();
		std::vector<std::string>	getNicknamesOpe(); //en cours ??? pas certaines de ce que je fais
		std::vector<std::string>	getNicknamesClients(); //en cours ??? pas certaines de ce que je fais
		std::string					getModes() const;
		std::string					getTopic() const;
		std::vector<Client *>		getClients() const;

		// SETTERS
		void			setPassword(const std::string password);
		void			setMaxclients(const size_t maxclients);
		void			setNbclients(const size_t nbclients);
		void			setNboperators(const size_t nboperators); //ok
		void			setModes(const std::string &modes);
		void			setTopic(const std::string &topic);
	
		// FCT MEMBRES
		void			addClient(Client *client);
		void			removeClient(Client *client);
		void			addOperator(Client *operators); //ok àtester
		void			removeOperator(Client *operators); //ok à tester +ajout quand -o à faire
		bool			client_is_operator(Client *client); //à faire puis tester;
		bool			client_is_inchannel(Client *client); // àtester
		void			sendall(const std::string& message);
		void			sendall(const std::string& message, Client *exclude);
		void			consolDeBUGchannel();

	private: 
		std::string				_name;
		std::string				_password;
		Client					*_admin;
		std::vector<Client *>	_clients;
		std::vector<Client *>	_operators;
		size_t					_maxclients;
		size_t					_nbclients;
		size_t					_nboperators; //ok
		//pour la gestion des modes:
		std::string				_modes;
		std::string				_topic;
};

#endif