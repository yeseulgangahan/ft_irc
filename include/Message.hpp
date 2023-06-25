#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <algorithm>
#include <unistd.h> 
#include <string> 
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <sys/socket.h>
#include "Client.hpp"

const std::string server_name = "ft_IRC";

#define REPLY(prefix, command, parameters, trailing) \
":" + (std::string)prefix + " " + \
(std::string)command + \
((std::string)parameters == "" ? (std::string)"" : (std::string)" " + (std::string)parameters) + \
((std::string)trailing == "" ? (std::string)"" : ((std::string)" :" +(std::string)trailing))


/* REGISTRATION */
// CAP LS
#define CAP_LS ":" + server_name + " CAP * LS"

// PASS
#define ERR_PASSWDMISMATCH(sender) RPL_NICK_MSG(sender, "464", "", "Password incorrect")

// NICK
#define ERR_ERRONEUSNICKNAME(sender) RPL_NICK_MSG(sender, "432", "", "Error one use nickname")
#define ERR_NICKNAMEINUSE(sender, new_nick) RPL_NICK_MSG(sender, "433", new_nick, "Nickname is already in use")
#define ERR_NOSUCHNICK(sender, target_nick) RPL_NICK_MSG(sender, "401", target_nick, "No such nick")
#define RPL_NICK_MSG(sender, command, parameters, trailing)	REPLY(server_name, command, sender.get_nick() + ((std::string)parameters == "" ? (std::string)"" : (" " + (std::string)parameters)), trailing)

// USER
#define ERR_ALREADYREGISTRED(sender) RPL_NICK_MSG(sender, "462", "", "You may not reregister")

// CAP END
#define RPL_WELCOME(client) REPLY(server_name, "001", client.get_nick(), "Welcome to the IRC Network " + client.get_user_info())
#define RPL_YOURHOST(client) REPLY(server_name, "002", client.get_nick(), "Your host is " + server_name + ", running version 1.0")
#define RPL_CREATED(client) REPLY(server_name, "003", client.get_nick(), "This server was created [getTime()]")
#define RPL_MYINFO(client) REPLY(server_name, "004", client.get_nick(), server_name + " 1.0 - -")


/* CONNECTION */
// PONG
#define PONG_MSG(arg) ":" + server_name + " PONG :" + arg

// QUIT

/* GENERAL */
#define REP_CMD(client, cmd) REPLY(client.get_user_info(), cmd.getWholeString(), "", "")
#define RPL_NONE(msg) REPLY(server_name, "300", "*", msg)
#define ERR_NEEDMOREPARAMS(sender, cmd_name) RPL_NICK_MSG(sender, "461", cmd_name, "Not enough parameters")
#define ERR_UNKNOWNCOMMAND(sender, cmd_name) RPL_NICK_MSG(sender, "421", cmd_name, "Unknown command")
#define ERR_NOTREGISTERED(sender) RPL_NICK_MSG(sender, "451", "", "You have not registered")


/* OPERATION */
// INVITE
#define RPL_INVITING(sender, target, ch) RPL_NICK_MSG(sender, "341", target.get_nick()+ " " + ch.getName(), "")
#define ERR_NOSUCHCHANNEL(sender, ch_name) RPL_NICK_MSG(sender, "403", ch_name, "No such channel")
#define ERR_USERONCHANNEL(sender, target, ch) RPL_NICK_MSG(sender, "443", target.get_nick()+" "+ch.getName(), "User is already on that channel")
#define ERR_CHANNELISFULL(sender, ch) RPL_NICK_MSG(sender, "471", ch.getName(), "Cannot join channel (+l)")

// JOIN
#define ERR_INVITEONLYCHAN(sender, ch) RPL_NICK_MSG(sender, "473", ch.getName(), + "Cannot join channel (+i)")
#define ERR_BADCHANNELKEY(sender, ch) RPL_NICK_MSG(sender, "475", ch.getName(), "Cannot join channel (+k)")

// TOPIC
#define RPL_NOTOPIC(sender, ch) RPL_NICK_MSG(sender, "331", "TOPIC " + ch.getName(), "No topic set for "+ ch.getName())
#define RPL_TOPIC(sender, ch, topic) RPL_NICK_MSG(sender, "332", ch.getName(), topic)

// MODE
#define RPL_CHANNELMODEIS(sender, ch, mode, target_nick) RPL_NICK_MSG(sender, "324", ch.getName() + ((std::string)mode == "" ? "" : " " + (std::string)mode) + ((std::string)target_nick == "" ? "" : " " + (std::string)target_nick), "")
#define ERR_NOCHANMODES(sender, mode) RPL_NICK_MSG(sender, "472", mode, "is an unknown mode character to me")
#define ERR_CHANOPRIVSNEEDED(ch) REPLY(":"+server_name, "482", ch.getName(), "You're not a channel operator")

// PRIVMSG
#define ERR_NORECIPIENT(sender, command) RPL_NICK_MSG(sender, "411", "", "No recipient given (" + command + ")")
#define ERR_NOTEXTTOSEND(sender) RPL_NICK_MSG(sender, "412", "", "No text to send")

// WHO
#define RPL_WHOREPLY(sender, ch, target) RPL_NICK_MSG(sender, "352", ch.getName() + " " + target.get_user_name() + " " + target.get_host_name() + " " + server_name+" " + target.get_nick()+ (std::string)" H" + ch.getMembershipString(target), "0 " + target.get_real_name())
#define RPL_ENDOFWHO(sender, ch) RPL_NICK_MSG(sender, "315", ch.getName(), "End of /WHO list")

//NAMES
#define RPL_NAMREPLY(sender, ch_name, name_list) RPL_NICK_MSG(sender, "353", ((std::string)ch_name == "*" ? "" : "= ") + ch_name ,name_list)
#define RPL_ENDOFNAMES(sender, ch_name) RPL_NICK_MSG(sender, "366", ch_name,"End of /NAMES list")

//KICK
#define ERR_USERNOTINCHANNEL(sender, target, ch) RPL_NICK_MSG(sender, "441", target.get_nick() + ch.getName(), "They aren't on that channel")
#define ERR_NOTONCHANNEL(sender, ch) RPL_NICK_MSG(sender, "442", ch.getName(), "You're not on that channel")


void reply(const Client &receiver, const std::string &message);
void sendWelcomeMessages(const Client &receiver);

#endif 