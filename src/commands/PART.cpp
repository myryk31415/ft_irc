# include "Server.hpp"


std::string splitCmdPart(std::vector<std::string>& cmd, std::vector<std::string>& channelsToPart)
{
	size_t pos;
	size_t comma_pos;
	std::string reason;
	std::string args = cmd[0];
	while ((comma_pos = args.find(',')) != std::string::npos)
	{
		channelsToPart.push_back(args.substr(0, comma_pos));
		args = args.substr(comma_pos + 1);
	}
	pos = args.find(' ');
	channelsToPart.push_back(args.substr(0, pos));
	reason = cmd[1];
	if (reason.empty())
		reason = "Gotta gooo bye bye";
	return reason;
}

void Server::PART(std::vector<std::string> cmd, int fd)
{
	std::vector<std::string> channelsToPart;
	Client &sender = *getClient(fd);
	if (cmd.size() < 1)
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNickname(), (cmd.empty() ? "PART" : "PART " + cmd[0])), fd); return ;}
	std::string reason = splitCmdPart(cmd, channelsToPart);

	for (auto it = channelsToPart.begin(); it != channelsToPart.end(); it++)
	{
		if (_channels.find((*it).substr(1)) == _channels.end())
			{sendResponse(ERR_NOSUCHCHANNEL(sender.getNickname(), (*it).substr(1)), fd); continue;}
		Channel &curChannel = _channels[(*it).substr(1)];
		if (!curChannel.getUser(sender.getNickname())) // check if sender is in channel
			{sendResponse(ERR_NOTONCHANNEL(sender.getNickname(), (*it).substr(1)), fd); continue ;}
		std::stringstream ss;
		ss << ":" << sender.getNickname() << "!" << sender.getUsername() << "@" << "localhost" << " PART #" << curChannel.getName() << " :" << reason;
		curChannel.systemMessage(ss.str());
		curChannel.removeUser(sender);
		if (curChannel.getOperator(sender.getNickname()))
			curChannel.removeOperator(sender);
		if (curChannel.getUserCount() == 0)
			_channels.erase(curChannel.getName());
	}
}
