# include "Server.hpp"

bool Server::checkDuplicate(std::string nick)
{
	bool found = false;
	for (auto &user : _clients)
	{
		found = (user.second.getNick() == nick);
	}
	return found;
}

bool checkValidNick(std::string nick)
{
	std::regex pattern("^[a-zA-Z0-9{}\\[\\]\\\\|]+$");
	return (std::regex_match(nick, pattern));
}

void	Server::NICK(std::vector<std::string> cmd, int fd)
{
	Client &sender = _clients[fd];
	if (cmd.empty() || cmd[0].empty())
		{sendResponse(ERR_NONICKNAMEGIVEN(sender.getNick()), fd); return ;}
	if (checkDuplicate(cmd[0]))
		{sendResponse(ERR_NICKNAMEINUSE(sender.getNick(), cmd[0]), fd); return ;}
	if (checkValidNick(cmd[0]))
		{sendResponse(ERR_ERRONEUSNICKNAME(sender.getNick(), cmd[0]), fd); return ;}
	std::stringstream ss;
	ss << ":" << sender.getNick() << "!" << sender.getUsername() << "@" << "localhost" << " NICK " << cmd[0];
	sender.receiveMsg(ss.str());
}

