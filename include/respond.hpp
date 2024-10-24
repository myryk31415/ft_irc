#pragma once
// Registration
# define RPL_WELCOME(client, networkname, nick) (":001 " + client + " :Welcome to the " + networkname + ", " + nick) //todo  !user@host
# define RPL_YOURHOST(client, servername, version) (":002 " + client + " :Your host is " + servername + ", running version " + version)
# define RPL_CREATED(client, datetime) (":003 " + client + " :This server was created " + datetime)
# define RPL_MYINFO(client, servername, version, usermodes, channelmodes) (":004 " + client + " " + servername + " " + version + " " + usermodes + " " + channelmodes) //todo more stuff
# define RPL_ISUPPORT(client, datetime) (":005 ")

// Replies
# define RPL_CHANGEDMODE(nick, host, channel, modes, args) (":" + nick + "!" + host + "@localhost MODE " + (channel[0] == '#' ? "" : "#") + channel + " " + modes + " " + args)
# define RPL_CHANNELMODEIS(nick, channel, modes, mode_values) (":324 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " " + modes + " " + mode_values)
# define RPL_NOTOPIC(nick, channel) (":331 " + nick + " " +  channel + " :No topic is set)
# define RPL_TOPIC(nick, channel, topic) (":332 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :" + topic)
# define RPL_TOPICWHOTIME(nick, channel, senderTime) (":333 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " " + senderTime)
# define RPL_INVITING(nick, user_nick, channel) (":341 "+ nick + " " + user_nick + " " + (channel[0] == '#' ? "" : "#") + channel)
# define RPL_NAMREPLY(nick, channel, allUsers) (":353 " + nick + " = " + (channel[0] == '#' ? "" : "#") + channel + " :" + allUsers)
# define RPL_ENDOFNAMES(nick, channel) (":366 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " " + ":End of /NAMES list")

// Errors
# define ERR_NOSUCHNICK(nick, user_nick) (":401 " + nick + " " + user_nick + " :No such nick/channel")
# define ERR_NOSUCHCHANNEL(nick, channel) (":403 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :No such channel")
# define ERR_NORECIPIENT(nick) (":411 " + nick + " :No recipient given")
# define ERR_NOTEXTTOSEND(nick) (":412 " + nick + " :No text to send")
# define ERR_NONICKNAMEGIVEN(nick) (":431 " + nick + " :No nickname given")
# define ERR_ERRONEUSNICKNAME(nick, falseNick) (":432 " + nick + " " + falseNick + " :Erroneus nickname")
# define ERR_NICKNAMEINUSE(nick, falseNick) (":433 " + nick + " " + falseNick + " :Nickname already in use")
# define ERR_USERNOTINCHANNEL(nick, user_nick, channel) (":441 " + nick + " " + user_nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :They aren't on that channel")
# define ERR_NOTONCHANNEL(nick, channel) (":442 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :You're not on that channel")
# define ERR_USERONCHANNEL(nick, user_nick, channel) (":443 " + nick + " " + user_nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :is already on channel")
# define ERR_NOTREGISTERED(client) (":451 " + client + " :You have not registered")
# define ERR_NEEDMOREPARAMS(nick, cmd) (":461 " + nick + " " + cmd + " :Not enough parameters")
# define ERR_ALREADYREGISTERED(client) (":462 " + client + " :You may not reregister")
# define ERR_PASSWDMISMATCH(client) (":464 " + client + " :Password incorrect")
# define ERR_CHANNELISFULL(nick, channel) (":471 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :Cannot join channel (+l)")
# define ERR_INVITEONLYCHAN(nick, channel) (":473 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :Cannot join channel (+i)")
# define ERR_BADCHANNELKEY(nick, channel) (":475 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :Cannot join channel (+k)")
# define ERR_CHANOPRIVSNEEDED(nick, channel) (":482 " + nick + " " + (channel[0] == '#' ? "" : "#") + channel + " :You're not channel operator")
# define ERR_UMODEUNKOWNFLAG(nick) (":501 " + nick + " :Unknown MODE flag")
# define ERR_INVALIDMODEPARAM(nick, target, mode, param, description) (":696 " + nick + " " + target + " " + mode + " " + param + " :" + description)
