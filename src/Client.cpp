
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

void	Client::set_ip_addr(std::string ip_addr)
{
	_ip_addr = ip_addr;
}
