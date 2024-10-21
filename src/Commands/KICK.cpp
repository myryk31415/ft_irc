#include "Server.hpp"

std::string splitCmdKick(std::vector<std::string>& cmd, std::string &channel, std::vector<std::string>& usersToKick)
{
	size_t pos;
	size_t comma_pos;
	std::string reason;
	std::string args = cmd[1];
	while ((comma_pos = args.find(',')) != std::string::npos)
	{
		usersToKick.push_back(args.substr(0, comma_pos));
		args = args.substr(comma_pos + 1);
	}
	pos = args.find(' ');
	usersToKick.push_back(args.substr(0, pos));
	args = args.substr(pos + 1);
	if (!args.empty())
	{
		if (args[0] == ':')
			reason = args.substr(1);
		else
			reason = args.substr(0, args.find_first_of(' '));
	}
	if (reason.empty())
		reason = "Kick because lol";
	return reason;
}

void Server::KICK(std::vector<std::string> cmd, int fd)
{
	std::vector<std::string> usersToKick;
	Client &sender = *getClient(fd);
	if (cmd.size() < 2)
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNick(), (cmd.empty() ? "" : "KICK " + cmd[0])), fd); return ;}
	std::string channel = cmd[0];
	std::string reason = splitCmdKick(cmd, channel, usersToKick);
	if (_channels.find(channel) != _channels.end())
	{
		Channel &curChannel = _channels[channel];
		if (!curChannel.getUser(sender.getNick())) // check if sender is in channel
			{sendResponse(ERR_NOTONCHANNEL(sender.getNick(), channel), fd); return ;}
		if (!curChannel.getOperator(sender.getNick())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender.getNick(), channel), fd); return ;}
		for (auto it = usersToKick.begin(); it != usersToKick.end(); it++) // Iterate through all users to kick
		{
				if (!curChannel.getUser(*it)) // check if user to kick is in channel
					{sendResponse(ERR_USERNOTINCHANNEL(sender.getNick(), *it, channel), fd); continue;}
				std::stringstream ss;
				ss << ":" << sender.getNick() << "!" << curChannel.getUser(*it)->getUsername() << "@" << "localhost" << " KICK #" << curChannel.getName() << " " << curChannel.getUser(*it)->getNick() << " :" << reason;
				curChannel.broadcastMessage(ss.str(), sender);
				curChannel.removeUser(*curChannel.getUser(*it));
				if (curChannel.getOperator(*it))
					curChannel.removeOperator(*curChannel.getOperator(*it));
				// If users count == 0
		}
	}
	else
		sendResponse(ERR_NOSUCHCHANNEL(sender.getNick(), channel), fd);
}
