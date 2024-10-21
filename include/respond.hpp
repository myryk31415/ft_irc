#pragma once

# define CRLF "\r\n"

//Errors
# define ERR_NEEDMOREPARAMS(nick, cmd) (": 461 " + nick + " " + cmd + " :Not enough parameters" + CRLF)
# define ERR_NOTONCHANNEL(nick, channel) (": 442 " + nick + " #" + channel + " :You're not on that channel" + CRLF)
# define ERR_CHANOPRIVSNEEDED(nick, channel) (": 482 " + nick + " #" + channel + " :You're not channel operator" + CRLF)
# define ERR_USERNOTINCHANNEL(nick, user_nick, channel) (": 441 " + nick + " " + user_nick + " #" + channel + " :They aren't on that channel" + CRLF)
# define ERR_NOSUCHCHANNEL(nick, channel) (": 403 " + nick + " #" + channel + " :No such channel" + CRLF)
# define ERR_NOSUCHNICK(nick, user_nick) (": 401 " + nick + " " + user_nick + " :No such nick/channel" + CRLF)
# define ERR_UMODEUNKOWNFLAG(nick) (": 501 " + nick + " :Unknown MODE flag" + CRLF)
# define ERR_INVALIDMODEPARAM(nick, target, mode, param, description) (": 696 " + nick + " " + target + " " + mode + " " + param + " :" + description + CRLF)


//Repliies
# define RPL_CHANNELMODEIS(nick, channel, modes, mode_values) (": 324 " + nick + " #" + channel + " " + modes + " " + mode_values + CRLF)
# define RPL_CHANGEDMODE(host, channel, modes, args) (":" + host + " MODE #" + channel + " " + modes + " " + args)
