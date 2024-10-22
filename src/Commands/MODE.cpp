#include "Server.hpp"

void getVars(std::string &target, std::string &modestring, std::string &params, std::string &cmd)
{
	std::istringstream iss(cmd);

	iss >> target;
	iss >> modestring;
	size_t found = cmd.find_first_not_of(target + modestring + " \t\v");
	if (found != std::string::npos)
		params = cmd.substr(found);
}

void addMode(std::string& modeReport, char toAdd, bool sign)
{
	size_t lastPlus = modeReport.find_last_of('+');
	size_t lastMinus = modeReport.find_last_of('-');

	if (modeReport.empty() ||
		(sign && (lastPlus == std::string::npos || (lastMinus != std::string::npos && lastMinus > lastPlus))) ||
		(!sign && (lastMinus == std::string::npos || (lastPlus != std::string::npos && lastPlus > lastMinus))))
	{
		modeReport += (sign ? '+' : '-');
	}

	modeReport += toAdd;
}

void inviteOnly(bool sign, Channel &channel, std::string &modeReport)
{
	if (sign)
		channel.setMode(INVITE_ONLY, "", true);
	else
		channel.setMode(INVITE_ONLY, "", false);
	addMode(modeReport, 'i', sign);
}

void topicRestrict(bool sign, Channel &channel, std::string &modeReport)
{
	if (sign)
		channel.setMode(TOPIC_SETTABLE_BY_OPERATOR, "", true);
	else
		channel.setMode(TOPIC_SETTABLE_BY_OPERATOR, "", false);
	addMode(modeReport, 't', sign);
}

void Server::channelKey(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator argsIt, std::string &argsReport, int fd)
{
	if (sign)
	{
		std::string key = *argsIt;
		if (key.empty())
			{sendResponse(ERR_NEEDMOREPARAMS(getClient(fd)->getNick(), "(o)"), fd); return ;}
		if (key.find_first_of(" \t\v") != std::string::npos)
			{sendResponse(ERR_INVALIDMODEPARAM(getClient(fd)->getNick(), channel.getName(), "k", key, "Should be without Whitespaces"), fd); return ;}
		channel.setMode(KEY, key, true);
		argsReport += *argsIt;
		argsIt++;
		addMode(modeReport, 'k', sign);
	}
	else
	{
		if (channel.isModeSet(USER_LIMIT))
			channel.setMode(USER_LIMIT, "", false);
		addMode(modeReport, 'k', sign);
	}

}

void Server::operatorPriv(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator argsIt, std::string &argsReport, int fd)
{
	std::string test = *argsIt;
	if (test.empty())
		{sendResponse(ERR_NEEDMOREPARAMS(getClient(fd)->getNick(), "(o)"), fd); return ;}
	if (_clients.find(*argsIt) == _clients.end())
		{sendResponse(ERR_NOSUCHNICK(getClient(fd)->getNick() ,*argsIt), fd); return ;}
	Client &user = _clients[*argsIt];
	if (!channel.getUser(*argsIt))
		{sendResponse(ERR_USERNOTINCHANNEL(getClient(fd)->getNick(), *argsIt, channel.getName()), fd); return ;}
	if (sign)
		channel.addOperator(user);
	else
		channel.removeOperator(user);
	argsReport += *argsIt;
	argsIt++;
	addMode(modeReport, 'o', sign);
}

void Server::userLimit(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator argsIt, std::string &argsReport, int fd)
{
	if (sign)
	{
		std::string limit = *argsIt;
		if (limit.empty())
			{sendResponse(ERR_NEEDMOREPARAMS(getClient(fd)->getNick(), "(o)"), fd); return ;}
		if (limit.find_first_not_of("0123456789") != std::string::npos || std::atoi(limit.c_str()) <= 0)
			{sendResponse(ERR_INVALIDMODEPARAM(getClient(fd)->getNick(), channel.getName(), "i", limit, "Should be a Int Number"), fd); return ;}
		channel.setMode(USER_LIMIT, limit, true);
		argsReport += *argsIt;
		argsIt++;
	}
	else
		channel.setMode(USER_LIMIT, "0", false);

	addMode(modeReport, 'l', sign);
}

void Server::MODE(std::vector<std::string> cmd, int fd)
{
	std::string target;
	std::string modestring;
	std::string params;
	bool	sign;
	std::vector<std::string> modeArgs;
	Client &sender = *getClient(fd);
	std::string modeReport;
	std::string argsReport;
	if (cmd.empty())
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNick(), ""), fd); return;}
	auto cmdIt = cmd.begin();
	target = *cmdIt++;
	if (!target.empty() || target[0] != '#' || _channels.find(target.substr(1)) == _channels.end())
		{sendResponse(ERR_NOSUCHCHANNEL(sender.getNick(), target.substr(1)), fd); return;}
	if (cmdIt != cmd.end())
		modestring = *cmdIt++;
	Channel &channel = _channels[target.substr(1)];
	if (!channel.getUser(sender.getNick())) // check if sender is in channel
		{sendResponse(ERR_NOTONCHANNEL(sender.getNick(), target.substr(1)), fd); return ;}
	if (!modestring.empty())
		{sendResponse(RPL_CHANNELMODEIS(sender.getNick(), target.substr(1), channel.getModestring(), channel.getModesvalues()), fd); return ;}
	if (!channel.getOperator(sender.getNick())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender.getNick(), target.substr(1)), fd); return ;}
	for (auto pos = modestring.begin(); pos != modestring.end(); pos++)
	{
		if (*pos == '+' || *pos == '-')
			sign = (*pos == '+'); //if *pos == + then sign will be 1
		else if (*pos == 'i')
			inviteOnly(sign, channel, modeReport);
		else if (*pos == 't')
			topicRestrict(sign, channel, modeReport);
		else if (*pos == 'k')
			channelKey(sign, channel, modeReport, cmdIt, argsReport, fd);
		else if (*pos =='o')
			operatorPriv(sign, channel, modeReport, cmdIt, argsReport, fd);
		else if (*pos == 'l')
			userLimit(sign, channel, modeReport, cmdIt, argsReport, fd);
		else
			sendResponse(ERR_UMODEUNKOWNFLAG(sender.getNick()), fd);
	}
	channel.systemMessage(RPL_CHANGEDMODE(sender.getUsername(), channel.getName(), modeReport, argsReport));
}
