# include "Server.hpp"

void Server::INVITE(std::vector<std::string> cmd, int fd)
{
	Client		&sender = *(getClient(fd));
	if (cmd.size() < 2)
		{sendResponse(ERR_NEEDMOREPARAMS(sender.getNick(), (cmd.empty() ? "" : "INVITE " + cmd[0])), fd); return;}
	std::string target = cmd[0];
	std::string channelnick = cmd[1].substr(1);
	if (_channels.find(channelnick) == _channels.end())
		{sendResponse(ERR_NOSUCHCHANNEL(sender.getNick(), channelnick.substr(1)), fd); return;}
	Channel		&channel = _channels[channelnick];
	if (!channel.getUser(sender.getNick())) // check if sender is in channel
		{sendResponse(ERR_NOTONCHANNEL(sender.getNick(), channelnick.substr(1)), fd); return ;}
	if (!getClient(target))
		{sendResponse(ERR_NOSUCHNICK(sender.getNick(), target), fd); return ;}
	if (channel.isModeSet(INVITE_ONLY) && !channel.getOperator(sender.getNick())) // check if sender has perms
		{sendResponse(ERR_CHANOPRIVSNEEDED(sender.getNick(), channelnick.substr(1)), fd); return ;}
	if (channel.getUser(target))
		{sendResponse(ERR_USERONCHANNEL(sender.getNick(), target, channelnick), fd); return ;}
	Client		&toInv = *(channel.getUser(target));
	if (!channel.isUserInvited(toInv))
	{
		channel.inviteUser(toInv, sender);
		sendResponse(RPL_INVITING(sender.getNick(), target, channelnick), fd);
		toInv.receiveMsg(": " + sender.getUsername() + " INVITE " + target + " #" + channelnick);
	}
}
