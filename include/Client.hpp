
#pragma once

#include "ft_irc.hpp"

class Client
{
	private:
		int			_fd;
		std::string	_ip_addr;
	public:
		Client();
		~Client();

		int		get_fd() const;
		void	set_fd(int fd);
		void	set_ip_addr(std::string ip_addr);
};
