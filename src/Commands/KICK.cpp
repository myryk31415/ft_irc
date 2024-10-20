#include "Server.hpp"

std::string splitCmdKick(std::string cmd, std::string &channel, std::vector<std::string>& usersToKick)
{
	size_t pos;
	size_t comma_pos;
	std::string reason;
	pos = cmd.find_first_of(' ');
	if (pos != std::string::npos)
		cmd = cmd.substr(pos + 1);
	pos = cmd.find_first_of(' ');
	channel = cmd.substr(0, pos);
	if (channel[0] == '#')
		channel.erase(channel.begin());
	cmd = cmd.substr(pos + 1);
	pos = cmd.find_first_of(' ');
	std::string substr = cmd.substr(0, pos);
	while ((comma_pos = substr.find(',')) != std::string::npos)
	{
		usersToKick.push_back(substr.substr(0, comma_pos));
		substr = substr.substr(comma_pos + 1);
	}
	usersToKick.push_back(substr);
	cmd = cmd.substr(pos + 1);
	if (!cmd.empty())
	{
		if (cmd[0] == ':')
			reason = cmd.substr(1);
		else
			reason = cmd.substr(0, cmd.find_first_of(' '));
	}
	if (reason.empty())
		reason = "Kick because lol";
	return reason;
}

void Server::KICK(std::string cmd, int fd)
{
	std::vector<std::string> usersToKick;
	std::string channel;
	std::string reason = splitCmdKick(cmd, channel, usersToKick);
	Client *sender = getClient(fd);
	if (usersToKick.empty() || channel.empty())
		{sendResponse(ERR_NEEDMOREPARAMS(sender->getNick(), cmd), fd); return ;}
	if (_channels.find(channel) != _channels.end())
	{
		Channel *curChannel = _channels[channel];
		if (!curChannel->getUser(sender->getNick())) // check if sender is in channel
			{sendResponse(ERR_NOTONCHANNEL(sender->getNick(), channel), fd); return ;}
		if (!curChannel->getOperator(sender->getNick())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender->getNick(), channel), fd); return ;}
		for (auto it = usersToKick.begin(); it != usersToKick.end(); it++) // Iterate through all users to kick
		{
				if (!curChannel->getUser(*it)) // check if user to kick is in channel
					{sendResponse(ERR_USERNOTINCHANNEL(sender->getNick(), *it, channel), fd); continue;}
				std::stringstream ss;
				ss << ":" << sender->getNick() << "!" << curChannel->getUser(*it)->getUsername() << "@" << "localhost" << " KICK #" << curChannel->getName() << " " << curChannel->getUser(*it)->getNick() << " :" << reason;
				curChannel->broadcastMessage(ss.str(), sender);
				curChannel->removeUser(curChannel->getUser(*it));
				if (curChannel->getOperator(*it))
					curChannel->removeOperator(curChannel->getOperator(*it));
				// If users count == 0
		}
	}
	else
		sendResponse(ERR_NOSUCHCHANNEL(sender->getNick(), channel), fd);
}
