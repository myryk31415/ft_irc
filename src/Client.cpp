
#include "Client.hpp"

Client::Client()
{
}

Client::~Client()
{
}

int		Client::getFd() const
{
	return (_fd);
}

void	Client::setFd(int fd)
{
	_fd = fd;
}

void	Client::setIpAddr(const std::string ip_addr)
{
	_ip_addr = ip_addr;
}

const std::string&	Client::getNick(void) const
{
	return _nick;
}

void	Client::setNick(const std::string &new_nick)
{
	_nick = new_nick;
}

const std::string& Client::getUsername(void) const
{
	return _username;
}

void	Client::setUsername(const std::string &new_username)
{
	_username = new_username;
}

void	Client::joinChannel(Channel *channel)
{
	channel->addUser(*this);
}

void	Client::leaveChannel(Channel *channel)
{
	channel->removeUser(*this);
}

void	receiveMsg(const std::string &msg)
{

}
