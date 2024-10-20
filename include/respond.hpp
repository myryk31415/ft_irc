#pragma once

# define CRLF "\r\n"

//Errors
# define ERR_NEEDMOREPARAMS(nick, cmd) (": 461 " + nick + " " + cmd + " :Not enough parameters")
# define ERR_NOTONCHANNEL(nick, channel) (": 442 " + nick + " #" + channel + " :You're not on that channel")
# define ERR_CHANOPRIVSNEEDED(nick, channel) (": 482 " + nick + " #" + channel + " :You're not channel operator")
# define ERR_USERNOTINCHANNEL(nick, user_nick, channel) (": 441" + nick + " " + user_nick + " #" + channel + " :They aren't on that channel")
# define ERR_NOSUCHCHANNEL(nick, channel) (": 403" + nick + " #" + channel + " :No such channel")





//Repliies
# define RPL_CHANNELMODEIS(nick, channel, modes) (": 324 " + nick + " #" + channel + " " + modes + CRLF)
