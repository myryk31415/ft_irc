
#pragma once

#include "ft_irc.hpp"
#include "Channel.hpp"
#include <sys/socket.h>

class Channel;

class Client
{
	private:
		int			_fd;
		std::string	_ip_addr;
		std::string _nick;
		std::string _username;
		std::string _realname;
		int			_auth_stage; // 2 no pass; 1 no nick; 0 authenticated
	public:
		Client();
		~Client();

		int		getFd() const;
		void	setFd(int fd);
		void	setIpAddr(const std::string ip_addr);
		const std::string& getNickname(void) const;
		void	setNickname(const std::string &new_nick);
		const std::string& getUsername(void) const;
		void	setUsername(const std::string &new_username);
		const std::string& getRealname(void) const;
		void	setRealname(const std::string &new_realname);
		int		getAuth() const;
		void	setAuth(int auth_stage);
		void	joinChannel(Channel *channel);
		void	leaveChannel(Channel *channel);
		void	receiveMsg(const std::string& msg);
};
