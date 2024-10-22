# include "Server.hpp"

void Server::TOPIC(std::vector<std::string> cmd, int fd)
{
	std::string topic;
	Client &sender = *getClient(fd);
	if (cmd.empty())
	{sendResponse(ERR_NEEDMOREPARAMS(sender.getNick(), ""), fd); return;}
	std::string targetChannel = cmd[0];
	if (!targetChannel.empty() || targetChannel[0] != '#' || _channels.find(targetChannel.substr(1)) == _channels.end())
		{sendResponse(ERR_NOSUCHCHANNEL(sender.getNick(), targetChannel.substr(1)), fd); return;}
	Channel &channel = _channels[targetChannel.substr(1)];
	if (!channel.getUser(sender.getNick())) // check if sender is in channel
		{sendResponse(ERR_NOTONCHANNEL(sender.getNick(), targetChannel.substr(1)), fd); return ;}
	if (cmd.size() > 1)
		topic = cmd[1];
	if (topic.empty())
	{
		std::pair<std::string, std::string> curTopic = channel.getTopic();
		if (curTopic.first.empty())
			sendResponse(RPL_NOTOPIC(sender.getNick(), targetChannel), fd);
		else
			{sendResponse(RPL_TOPIC(sender.getNick(), targetChannel, curTopic.first) + RPL_TOPICWHOTIME(sender.getNick(), targetChannel, curTopic.second), fd); return;}
	}
	if (channel.isModeSet(TOPIC_SETTABLE_BY_OPERATOR) && !channel.getOperator(sender.getNick())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender.getNick(), targetChannel.substr(1)), fd); return ;}
	std::pair<std::string, std::string> newTopic;
	newTopic.first = topic;
	std::time_t t = std::time(nullptr);
	newTopic.second = sender.getNick() + " " + std::to_string(t);
	channel.setTopic(newTopic);
}
