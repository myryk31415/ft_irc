# include "Server.hpp"


void splitComma(std::string &cmd, std::vector<std::string> &split)
{
	std::istringstream iss(cmd);
	std::string line;
	while(std::getline(iss, line, ','))
		if (!line.empty())
			split.push_back(line);
}

void Server::leaveAllChannels(Client &sender)
{
	for (auto it = _channels.begin(); it != _channels.end();)
	{
		auto &curChannel = it->second;
		if (curChannel.getUser(sender.getNick()))
			curChannel.removeUser(sender);
		if (curChannel.getOperator(sender.getNick()))
			curChannel.removeOperator(sender);
		if (curChannel.getUserCount() == 0)
			it = _channels.erase(it);
		else
			it++;
	}
}

void Server::JOIN(std::vector<std::string> cmd, int fd)
{
	std::vector<std::string> channels;
	std::vector<std::string> keys;
	Client &sender = *getClient(fd);
	if (!cmd.empty() && cmd[0] == "0")
		{leaveAllChannels(sender); return ;}
	if (cmd.size() < 1)
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNick(), (cmd.empty() ? "" : "JOIN " + cmd[0])), fd); return ;}
	splitComma(cmd[0], channels);
	if (cmd.size() == 2)
		splitComma(cmd[1], keys);
	

	auto itKeys = keys.begin();
	for (auto itChannels = channels.begin(); itChannels != channels.end(); itChannels++)
	{
		if ((*itChannels)[0] != '#')
			{sendResponse(ERR_NOSUCHCHANNEL(sender.getNick(), (*itChannels).substr(1)), fd); continue;}
		// if (_channels.find((*itChannels).substr(1)) == _channels.end())
		std::string channelname = (*itChannels).substr(1);
		if (_channels.find(channelname) == _channels.end())
		{
			if (itKeys != keys.end())
				_channels.emplace(channelname, Channel(channelname, *itKeys));
			else
				_channels.emplace(channelname, Channel(channelname));
		}
		Channel &curChannel = _channels[channelname];
		if (curChannel.isModeSet(KEY) && itKeys != keys.end() && curChannel.getModeValue(KEY) != *itKeys)
			{sendResponse(ERR_BADCHANNELKEY(sender.getNick(), channelname), fd); continue;}
		if (curChannel.isModeSet(INVITE_ONLY) && !curChannel.isUserInvited(sender))
			{sendResponse(ERR_INVITEONLYCHAN(sender.getNick(), channelname), fd); continue;}
		if (curChannel.isModeSet(USER_LIMIT) && curChannel.isChannelfull())
			{sendResponse(ERR_CHANNELISFULL(sender.getNick(), channelname), fd); continue;}
		curChannel.addUser(sender);
		if (curChannel.getUserCount() == 1)
			curChannel.addOperator(sender);
		std::stringstream ss;
		ss << ":" << sender.getNick() << "!" << sender.getUsername() << "@" << "localhost" << " JOIN #" << curChannel.getName();
		curChannel.systemMessage(ss.str());
		auto curTopic = curChannel.getTopic();
		if (!curTopic.first.empty())
			sendResponse(RPL_TOPIC(sender.getNick(), *itChannels, curTopic.first) + RPL_TOPICWHOTIME(sender.getNick(), *itChannels, curTopic.second), fd);
		sendResponse(RPL_NAMREPLY(sender.getNick(), *itChannels, curChannel.getAllUsers()) + RPL_ENDOFNAMES(sender.getNick(), *itChannels), fd);
		if (curChannel.isModeSet(KEY))
			itKeys++;
	}
}
