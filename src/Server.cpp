/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:38:40 by padam             #+#    #+#             */
/*   Updated: 2024/10/16 22:12:54 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port) : _port(port) {}

Server::~Server() {}

void	Server::signal_handler(int signal)
{
	std::cout << std::endl << "Signal " << signal << " received!" << std::endl;
}

void	Server::init()
{
	// TODO throw errors?
	if (signal(SIGINT, signal_handler) == SIG_ERR) // "ctrl + c"
		return;
	if (signal(SIGQUIT, signal_handler) == SIG_ERR); // "ctrl + \"
		return;

	_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0); // create IPv4 TCP socket
	if (_server_socket_fd == -1)
		return;

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = _port;
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(_server_socket_fd, (struct sockaddr *)&addr, sizeof(sockaddr_in)) == -1)
		return;
	// maybe setsockopt and fnctl
	if (listen(_server_socket_fd, 10) == -1) // wait for connections, 10 is arbitrary
		return;

	//polling the socket needs to be done

}
