/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aweizman <aweizman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:38:31 by padam             #+#    #+#             */
/*   Updated: 2024/10/16 16:30:22 by aweizman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <vector>
# include "Client.hpp"
# include "Channel.hpp"
# include <poll.h>
# include <signal.h>
# include <sys/socket.h>
class Server
{
	private:
		static bool _Signal;
		int _port;
		int _server_sock_fd;
		std::vector<struct pollfd> _socks;
		std::vector<Client> _clients;
	public:
		Server();
		~Server();
};
