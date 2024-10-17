/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:38:31 by padam             #+#    #+#             */
/*   Updated: 2024/10/17 16:57:28 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <poll.h>
#include <csignal>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>
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
	public:
		Server() = delete;
		Server(int port);
		~Server();

		static void	signal_handler(int signal);

		void		init();
		void		setup_server_socket();
		void		shutdown();
		void		poll();
		void		accept_client();
		void		receive_data();
};
