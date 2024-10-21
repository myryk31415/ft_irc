
#pragma once

#include <vector>
#include <poll.h>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>
#include <forward_list>
#include "ft_irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "respond.hpp"
class Server
{
	private:
		static bool _signal;
		int _port;
		int _server_socket_fd;
		std::vector<struct pollfd> _sockets;
		std::unordered_map<std::string, Client> _clients;
		std::unordered_map<std::string, Channel> _channels;
		std::string			_name;
	public:
		Server(int port);
		~Server();

		static void	signalHandler(int signal);

		void		init();
		void		setupServerSocket();
		void		shutdown();
		void		clearClient(int fd);
		void		poll();
		void		acceptClient();
		Client*		getClient(int fd);
		void		receiveData(int fd);
		// void		sendError(std::string numeric, std::string client, std::string msg, int fd);
		// void		sendError(std::string numeric, std::string client, std::string channel, std::string msg, int fd);
		template <typename... Args>
		void		sendError(std::string numeric, int fd, std::string client, Args... args);
		void		sendResponse(std::string message, int fd);
		// void		parseCommand(const std::string command);
		void		userLimit(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator argsIt, std::string &argsReport, int fd);
		void		channelKey(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator argsIt, std::string &argsReport, int fd);
		void		operatorPriv(bool sign, Channel &channel, std::string &modeReport, std::vector<std::string>::iterator args_it, std::string &argsReport, int fd);
		//Commands
		void KICK(std::string cmd, int fd);
		void MODE(std::string cmd, int fd);
void	INVITE(std::vector<std::string> cmd, int fd)
};
