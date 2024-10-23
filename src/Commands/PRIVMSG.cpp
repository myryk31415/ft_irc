#include "Server.hpp"

void splitComma(std::string &cmd, std::vector<std::string> &split)
{
	std::istringstream iss(cmd);
	std::string line;
	while(std::getline(iss, line, ','))
		if (!line.empty())
			split.push_back(line);
}

void	Server::PRIVMSG(std::vector<std::string> cmd, int fd)
{
	Client &sender = _clients[fd];
	if (cmd.empty() || cmd[0].empty())
		{sendResponse(ERR_NORECIPIENT(sender.getNick()), fd); return ;}
	if (cmd.size() < 2)
		{sendResponse(ERR_NOTEXTTOSEND(sender.getNick()), fd); return ;}
	std::vector<std::string> targetsToSend;
	splitComma(cmd[0], targetsToSend);
	for (auto &target : targetsToSend)
	{
		if (!getClient(target) && (target[0] == '#' && _channels.find(target.substr(1)) != _channels.end()))
			{sendResponse(ERR_NOSUCHNICK(sender.getNick(), target), fd); return ;}
		if (getClient(target))
			getClient(target)->receiveMsg(":" + sender.getNick() + " NICK " + target + " :" + cmd[1]);
		else
			_channels[target.substr(1)].systemMessage( ":" + sender.getNick() + "!" + sender.getUsername() + "@" + "localhost" + " PRIV #" + target.substr(1) + " :" + cmd[1]);
	}
}
