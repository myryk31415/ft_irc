#pragma once

# define CRLF "\r\n"

// Registration
# define RPL_WELCOME(client, networkname, nick) (": 001 " + client + " :Welcome to the " + networkname + ", " + nick + CRLF) //todo  !user@host
# define RPL_YOURHOST(client, servername, version) (": 002 " + client + " :Your host is " + servername + ", running version " + version + CRLF)
# define RPL_CREATED(client, datetime) (": 003 " + client + " :This server was created " + datetime + CRLF)
# define RPL_MYINFO(client, servername, version, usermodes, channelmodes) (": 004 " + client + " " + servername + " " + version + " " + usermodes + " " + channelmodes + CRLF) //todo more stuff
# define RPL_ISUPPORT(client, datetime) (": 005 " + CRLF)

// Replies
# define RPL_CHANGEDMODE(host, channel, modes, args) (":" + host + " MODE #" + channel + " " + modes + " " + args+ CRLF)
# define RPL_CHANNELMODEIS(nick, channel, modes, mode_values) (": 324 " + nick + " #" + channel + " " + modes + " " + mode_values + CRLF)
# define RPL_NOTOPIC(nick, channel) (": 331 " + nick + " " +  channel + " :No topic is set"+ CRLF)
# define RPL_TOPIC(nick, channel, topic) (": 332 " + nick + " " + channel + " :" + topic + CRLF)
# define RPL_TOPICWHOTIME(nick, channel, senderTime) (": 333 " + nick + " " + channel + " " + senderTime + CRLF)
# define RPL_INVITING(nick, user_nick, channel) (": 341 "+ nick + " " + user_nick + " " + channel + CRLF)
# define RPL_NAMREPLY(nick, channel, allUsers) (": 353 " + nick + " = " + channel + " :" + allUsers + CRLF)
# define RPL_ENDOFNAMES(nick, channel) (": 366 " + nick + " " + channel + " " + ":End of /NAMES list" + CRLF)

// Errors
# define ERR_NOSUCHNICK(nick, user_nick) (": 401 " + nick + " " + user_nick + " :No such nick/channel" + CRLF)
# define ERR_NOSUCHCHANNEL(nick, channel) (": 403 " + nick + " #" + channel + " :No such channel" + CRLF)
# define ERR_NORECIPIENT(nick) (": 411 " + nick + " :No recipient given" + CRLF)
# define ERR_NOTEXTTOSEND(nick) (": 412 " + nick + " :No text to send" + CRLF)
# define ERR_NONICKNAMEGIVEN(nick) (": 431 " + nick + " :No nickname given" + CRLF)
# define ERR_ERRONEUSNICKNAME(nick, falseNick) (": 432 " + nick + " " + falseNick + " :Erroneus nickname" + CRLF)
# define ERR_NICKNAMEINUSE(nick, falseNick) (": 433 " + nick + " " + falseNick + " :Nickname already in use" + CRLF)
# define ERR_USERNOTINCHANNEL(nick, user_nick, channel) (": 441 " + nick + " " + user_nick + " #" + channel + " :They aren't on that channel" + CRLF)
# define ERR_NOTONCHANNEL(nick, channel) (": 442 " + nick + " #" + channel + " :You're not on that channel" + CRLF)
# define ERR_USERONCHANNEL(nick, user_nick, channel) (": 443 " + nick + " " + user_nick + " #" + channel + " :is already on channel" + CRLF)
# define ERR_NOTREGISTERED(client) (": 451 " + client + " :You have not registered" + CRLF)
# define ERR_NEEDMOREPARAMS(nick, cmd) (": 461 " + nick + " " + cmd + " :Not enough parameters" + CRLF)
# define ERR_ALREADYREGISTERED(client) (": 462 " + client + " :You may not reregister" + CRLF)
# define ERR_PASSWDMISMATCH(client) (": 464 " + client + " :Password incorrect" + CRLF)
# define ERR_CHANNELISFULL(nick, channel) (": 471 " + nick + " " + channel + " :Cannot join channel (+l)" + CRLF)
# define ERR_INVITEONLYCHAN(nick, channel) (": 473 " + nick + " " + channel + " :Cannot join channel (+i)" + CRLF)
# define ERR_BADCHANNELKEY(nick, channel) (": 475 " + nick + " " + channel + " :Cannot join channel (+k)" + CRLF)
# define ERR_CHANOPRIVSNEEDED(nick, channel) (": 482 " + nick + " #" + channel + " :You're not channel operator" + CRLF)
# define ERR_UMODEUNKOWNFLAG(nick) (": 501 " + nick + " :Unknown MODE flag" + CRLF)
# define ERR_INVALIDMODEPARAM(nick, target, mode, param, description) (": 696 " + nick + " " + target + " " + mode + " " + param + " :" + description + CRLF)
