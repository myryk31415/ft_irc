
#include "Client.hpp"

Client::Client()
{
}

Client::~Client()
{
}

int		Client::get_fd() const
{
	return (_fd);
}

void	Client::set_fd(int fd)
{
	_fd = fd;
}

void	Client::set_ip_addr(const std::string ip_addr)
{
	_ip_addr = ip_addr;
}

const std::string&	Client::getNick(void) const
{

}


void	Client::setNick(const std::string &new_nick) const
{

}


void	Client::joinChannel(Channel *channel)
{
	channel->addUser(this);
}

void	Client::leaveChannel(Channel *channel)
{
	channel->removeUser(this);
}

void	receiveMsg(const std::string &msg)
{

}
