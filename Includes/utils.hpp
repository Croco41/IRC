#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>


//Messages d'erreur

//Réponses du server
#define RPL_WELCOME(source)		"001 " + source + " :Welcome to the ft_irc network " + source
#define RPL_NAMREPLY(source, channel, users)	"353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)			"366 " + source + " " + channel + " :End of /NAMES list."
//Réponses suite aux commandes

#endif