#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>

#include "colors.hpp"

//Messages d'erreur
#define ERR_NOSUCHNICK(source, target)			"401 " + source + " " + target + " :No such nickname / channel"
#define ERR_NOSUCHCHANNEL(source, target)		"403 " + source + " " + target + " :Channel name is invalid, or does not exist"
#define ERR_CANNOTSENDTOCHAN(source, target)	"404 " + source + " " + target + " :Cannot send a message to channel"
#define ERR_TOOMANYCHANNELS(source, channel)	"405 " + source + " " + channel + " :You have joined too many channels"
#define ERR_UNKNOWNCOMMAND(source, command)		"421 " + source + " " + command + " :Command unknown"
#define ERR_NONICKNAMEGIVEN(source, command)	"431 " + source + " " + command + " :No new nickname has been given"
#define ERR_NICKNAMEINUSE(source)				"433 " + source + " :This nickname is already in use"
#define ERR_NOTONCHANNEL(source, channel)		"442 " + source + " " + channel + " :Client is not a member of this channel"
#define ERR_NEEDMOREPARAMS(source, command)		"461 " + source + " " + command + " :Not enough parameters"
#define ERR_ALREADYREGISTERED(source)			"462 " + source + " :You may not reregister, you already have!"
#define ERR_CHANNELISFULL(source, channel)		"471 " + source + " " + channel + " :This channel can't accept more clients"
#define ERR_BADCHANNELKEY(source, channel)		"475 " + source + " " + channel + " :Cannot join channel, please verify your password"


//Réponses du server
#define RPL_WELCOME(source)						"001 " + source + " :Welcome to the ft_irc network " + source
#define RPL_NAMREPLY(source, channel, users)    "353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)			"366 " + source + " " + channel + " :End of /NAMES list."

//Réponses suite aux commandes (custom)
#define RPL_JOIN(source, channel)				":" + source + " JOIN :" + channel	
#define RPL_PING(source, command)				":" + source + " PONG :" + command
#define RPL_PRIVMSG(source, target, message)	":" + source + " PRIVMSG " + target + " :" + message
#define RPL_PART(source, channel, message)		":" + source + " PART " + channel + " " + message
#define RPL_NOTICE(source, target, message)		":" + source + " NOTICE " + target + " :" + message

#endif