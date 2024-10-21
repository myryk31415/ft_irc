#include "Server.hpp"

std::vector<std::string>::iterator splitParams(std::string params, std::vector<std::string> &modeArgs)
{
	size_t found;
	while ((found = params.find(' ')) != std::string::npos)
	{
		modeArgs.push_back(params.substr(0, found));
		params = params.substr(found + 1);
	}
	return (modeArgs.begin());
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

void invite_only(bool sign, Channel &channel, std::string &modeReport)
{
	if (sign)
		channel.setMode(INVITE_ONLY, "", true);
	else
		channel.setMode(INVITE_ONLY, "", false);
	addMode(modeReport, 'i', sign);
}

void topic_restict(bool sign, Channel &channel, std::string &modeReport)
{
	if (sign)
		channel.setMode(TOPIC_SETTABLE_BY_OPERATOR, "", true);
	else
		channel.setMode(TOPIC_SETTABLE_BY_OPERATOR, "", false);
	addMode(modeReport, 't', sign);
}

void channel_key(bool sign, Channel &channel, std::string &modeReport)
{

}

void Server::operatorPriv(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator args_it, std::string &argsReport, int fd)
{
	std::string test = *args_it;
	if (test.empty())
		{sendResponse(ERR_NEEDMOREPARAMS(getClient(fd)->getNick(), "(o)"), fd); return ;}
	if (_clients.find(*args_it) == _clients.end())
		{sendResponse(ERR_NOSUCHNICK(getClient(fd)->getNick() ,*args_it), fd); return ;}
	Client &user = _clients[*args_it];
	if (!channel.getUser(*args_it))
		{sendResponse(ERR_USERNOTINCHANNEL(getClient(fd)->getNick(), *args_it, channel.getName()), fd); return ;}
	if (sign)
		channel.addOperator(user);
	else
		channel.removeOperator(user);
	argsReport += *args_it;
	args_it++;
	addMode(modeReport, 'o', sign);
}

void user_limit(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator args_it, std::string &argsReport, int fd)
{

}

void Server::MODE(std::string cmd, int fd)
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
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNick(), cmd), fd); return;}
	size_t found = cmd.find_first_not_of("MODEmode \t\v");
	if (found != std::string::npos)
		cmd = cmd.substr(found);
	else
		sendResponse(ERR_NEEDMOREPARAMS(sender.getNick(), cmd), fd);
	getVars(target, modestring, params, cmd);
	auto args_it = splitParams(params, modeArgs);
	if (!target.empty() || target[0] != '#' || _channels.find(target.substr(1)) == _channels.end())
		{sendResponse(ERR_NOSUCHCHANNEL(sender.getNick(), target.substr(1)), fd); return;}
	Channel &channel = _channels[target.substr(1)];
	if (!channel.getUser(sender.getNick())) // check if sender is in channel
		{sendResponse(ERR_NOTONCHANNEL(sender.getNick(), target.substr(1)), fd); return ;}
	if (!modestring.empty())
		{sendResponse(RPL_CHANNELMODEIS(sender.getNick(), target.substr(1), channel.getModestring(), channel.getModesvalues()), fd); return ;}
	if (!channel.getOperator(sender.getNick())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender.getNick(), target.substr(1)), fd); return ;}
	for (auto pos = modestring.begin(); pos != modestring.end(); pos)
	{
		if (*pos == '+' || *pos == '-')
			sign = (*pos == '+'); //if *pos == + then sign will be 1
		if (*pos == 'i')
			invite_only(sign, channel, modeReport);
		if (*pos == 't')
			topic_restict(sign, channel, modeReport);
		if (*pos == 'k')
			channel_key(sign, channel, modeReport);
		if (*pos =='o')
			operatorPriv(sign, channel, modeReport, args_it, argsReport, fd);
		if (*pos == 'l')
			user_limit(sign, channel, modeReport, args_it, argsReport, fd);
	}
	channel.systemMessage(modeReport);
}
