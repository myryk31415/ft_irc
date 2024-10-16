/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:38:31 by padam             #+#    #+#             */
/*   Updated: 2024/10/16 21:44:41 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector>
# include <poll.h>
# include <csignal>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include "ft_irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"

struct sockaddr_in {
    sa_family_t    sin_family; /* address family: AF_INET */
    in_port_t      sin_port;   /* port in network byte order */
    struct in_addr sin_addr;   /* internet address */
};

class Server
{
	private:
		static bool _Signal;
		int _port;
		int _server_socket_fd;
		std::vector<struct pollfd> _socks;
		std::vector<Client> _clients;
	public:
		Server() = delete;
		Server(int port);
		~Server();

		static void	signal_handler(int signal);

		void		init();
		void		create_socket();
};
