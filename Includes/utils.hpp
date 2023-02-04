#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>

#include "colors.hpp"

//Messages d'erreur
#define ERR_NEEDMOREPARAMS(source, command)		"461 " + source + " " + command + " :Not enough parameters"
#define ERR_TOOMANYCHANNELS(source, channel)	"405 " + source + " " + channel + " :You have joined too many channels"
#define ERR_CHANNELISFULL(source, channel)		"471 " + source + " " + channel + " :This channel can't accept more clients"
#define ERR_BADCHANNELKEY(source, channel)		"475 " + source + " " + channel + " :Cannot join channel, please verify your password"
#define ERR_UNKNOWNCOMMAND(source, command)		"421 " + source + " " + command + " :Command unknown"

//Réponses du server
#define RPL_WELCOME(source)						"001 " + source + " :Welcome to the ft_irc network " + source
#define RPL_NAMREPLY(source, channel, users)    "353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)			"366 " + source + " " + channel + " :End of /NAMES list."

//Réponses suite aux commandes
#define RPL_JOIN(source, channel)				":" + source + " JOIN :" + channel	
#define RPL_PING(source, command)				":" + source + " PONG :" + command

#endif