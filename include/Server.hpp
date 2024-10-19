
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
#include "ft_irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	private:
		static bool _signal;
		int _port;
		int _server_socket_fd;
		std::vector<struct pollfd> _sockets;
		std::vector<Client> _clients;
		std::unordered_map<std::string, Channel *> _channels;
	public:
		Server(int port);
		~Server();

		static void	signal_handler(int signal);

		void		init();
		void		setup_server_socket();
		void		shutdown();
		void		clear_client(int fd);
		void		poll();
		void		accept_client();
		Client *	getClient(int fd);
		void		receive_data(int fd);

		//Commands
		void KICK(std::string cmd, int fd);
};
