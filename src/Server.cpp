/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:38:40 by padam             #+#    #+#             */
/*   Updated: 2024/10/17 10:34:33 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::_signal = false;

Server::Server(int port) : _port(port) {}

Server::~Server() {}

void	Server::signal_handler(int signal)
{
	std::cout << std::endl << "Signal " << signal << " received!" << std::endl;
	_signal = true;
}

/**
 * @brief sets up signal handlers and the server socket.
 * on error the server gets shut down cleanly
 */
void	Server::init()
{
	try {
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, signal_handler);
		// if (signal(SIGINT, signal_handler) == SIG_ERR) // "ctrl + c"
		// 	throw(std::runtime_error("setting signal handler failed"));
		// if (signal(SIGQUIT, signal_handler) == SIG_ERR); // "ctrl + \"
		// 	throw(std::runtime_error("setting signal handler failed"));

		setup_server_socket();
		std::cout << GREEN << "Server " << _server_socket_fd << " started" << RESET << std::endl;
		std::cout << "Waiting for connections" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << RED << "Error while initializing server:" << std::endl;
		std::cout << e.what() << RESET << std::endl;
		shutdown();
	}
}

void	Server::setup_server_socket()
{
	_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0); // create IPv4 TCP socket
	if (_server_socket_fd == -1)
		throw(std::runtime_error("socket creation failed"));

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port); // convert from host to network byte order
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(_server_socket_fd, (struct sockaddr *)&addr, sizeof(sockaddr_in)) == -1)
		throw(std::runtime_error("socket binding failed"));
	int value = 1;
	if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
		throw(std::runtime_error("setting socket option `REUSEADDR` failed"));
	if (fcntl(_server_socket_fd, F_SETFL, O_NONBLOCK) == -1) // make fd nonblocking for MacOS
		throw(std::runtime_error("setting socket to `NONBLOCK` failed"));
	if (listen(_server_socket_fd, SOMAXCONN) == -1) // wait for connections
		throw(std::runtime_error("socket listening failed"));

	struct pollfd new_poll;
	new_poll.fd = _server_socket_fd;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	_sockets.push_back(new_poll);
}

void	Server::shutdown()
{
	std::cout << RED;
	for(auto iter = _clients.begin(); iter != _clients.end(); iter++)
	{
		std::cout << "Disconnecting client " << (*iter).get_fd() << std::endl;
		close((*iter).get_fd());
	}
	if (_server_socket_fd != -1)
	{
		std::cout << "Shutting down server " << _server_socket_fd << std::endl;
		close(_server_socket_fd);
	}
	std::cout << RESET;
}

void	Server::poll()
{
	while (_signal == false)
	{
		if (::poll(&_sockets[0], _sockets.size(), -1) == -1 && !_signal)
			throw(std::runtime_error("polling failed"));
		for (auto iter = _sockets.begin(); iter != _sockets.end(); iter++)
		{
			if ((*iter).revents & POLLIN)
			{
				if ((*iter).fd == _server_socket_fd)
					accept_client();
				else
					receive_data();
			}
		}
	}
	shutdown();
}

void	Server::accept_client()
{

}

void	Server::receive_data()
{

}
