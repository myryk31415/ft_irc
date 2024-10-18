
#pragma once

#include "ft_irc.hpp"
#include "Channel.hpp"

class Channel;

class Client
{
	private:
		int			_fd;
		std::string	_ip_addr;
		std::string nick;
	public:
		Client();
		~Client();

		int		get_fd() const;
		void	set_fd(int fd);
		void	set_ip_addr(const std::string ip_addr);
		const std::string& getNick(void) const;
		void	setNick(const std::string &new_nick) const;
		void	joinChannel(Channel *channel);
		void	leaveChannel(Channel *channel);
		void	receiveMsg(const std::string& msg);
};
