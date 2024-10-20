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

void Server::MODE(std::string cmd, int fd)
{
	std::string target;
	std::string modestring;
	std::string params;
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
	
}
