#include "Server.hpp"

void splitParams(std::string params, std::vector<std::string> &modeArgs)
{
	size_t found;
	while ((found = params.find(',')) != std::string::npos)
	{
		modeArgs.push_back(params.substr(0, found));
		params = params.substr(found + 1);
	}
}

void getVars(std::string &target, std::string &modestring, std::string &params, std::string &cmd)
{
	std::istringstream iss(cmd);

	iss >> target;
	iss >> modestring;
	size_t found = cmd.find_first_not_of(target + modestring + " \t\v");
	if (found != std::string::npos)
		params = cmd.substr(found);
}

void invite_only(int sign, Channel* channel, int fd)
{

}

void Server::MODE(std::string cmd, int fd)
{
	std::string target;
	std::string modestring;
	std::string params;
	int			sign;
	std::vector<std::string> modeArgs;
	Client * sender = getClient(fd);

	if (cmd.empty())
		{sendResponse(ERR_NEEDMOREPARAMS(sender->getNick(), cmd), fd); return;}
	size_t found = cmd.find_first_not_of("MODEmode \t\v");
	if (found != std::string::npos)
		cmd = cmd.substr(found);
	else
		sendResponse(ERR_NEEDMOREPARAMS(sender->getNick(), cmd), fd);
	getVars(target, modestring, params, cmd);
	splitParams(params, modeArgs);
	if (!target.empty() || target[0] != '#' || _channels.find(target.substr(1)) == _channels.end())
		{sendResponse(ERR_NOSUCHCHANNEL(sender->getNick(), target.substr(1)), fd); return;}
	Channel *channel = _channels[target.substr(1)];
	if (!channel->getUser(sender->getNick())) // check if sender is in channel
		{sendResponse(ERR_NOTONCHANNEL(sender->getNick(), target.substr(1)), fd); return ;}
	if (!modestring.empty())
		{sendResponse(RPL_CHANNELMODEIS(sender->getNick(), target.substr(1), channel->getModestring(), channel->getModesvalues()), fd); return ;}
	if (!channel->getOperator(sender->getNick())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender->getNick(), target.substr(1)), fd); return ;}
	for (auto pos = modestring.begin(); pos != modestring.end(); pos)
	{
		if (*pos == '+' || *pos == '-')
			sign = (*pos == '+');
		if (*pos == 'i')
			invite_only(sign, channel, fd);
		if (*pos == 't')
			topic_restict(sign, channel, fd);
		if (*pos == 'k')
			channel_key(sign, channel, fd);
		if (*pos =='o')
			operator_priv(sign, channel, fd);
		if (*pos == 'l')
			user_limit(sign, channel, fd);
	}

}
