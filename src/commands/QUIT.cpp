#include "Server.hpp"

void	Server::QUIT(std::vector<std::string> cmd, int fd)
{
	std::string msg;

	msg = "Quit: ";
	if (cmd.size())
		msg.append(cmd[0]);

	_clients.erase(fd);
	systemMessage(msg);
}
