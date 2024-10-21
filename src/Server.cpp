
#include "Server.hpp"

bool Server::_signal = false;

Server::Server(int port) : _port(port) {}

Server::~Server() {}

void	Server::signalHandler(int signal)
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
		signal(SIGINT, signalHandler);
		signal(SIGQUIT, signalHandler);
		// if (signal(SIGINT, signalHandler) == SIG_ERR) // "ctrl + c"
		// 	throw(std::runtime_error("setting signal handler failed"));
		// if (signal(SIGQUIT, signalHandler) == SIG_ERR); // "ctrl + \"
		// 	throw(std::runtime_error("setting signal handler failed"));

		setupServerSocket();
		std::cout << GREEN << "Server " << _server_socket_fd << " started" << RESET << std::endl;
		std::cout << "Waiting for connections" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << RED << "Error while initializing server:" << std::endl;
		std::cout << e.what() << RESET << std::endl;
		shutdown();
		throw(e);
	}
}

void	Server::setupServerSocket()
{
	_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0); // create IPv4 TCP socket
	if (_server_socket_fd == -1)
		throw(std::runtime_error("server socket creation failed"));

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port); // convert from host to network byte order
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(_server_socket_fd, (struct sockaddr *)&addr, sizeof(sockaddr_in)) == -1)
		throw(std::runtime_error("server socket binding failed"));
	int value = 1;
	if (setsockopt(_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
		throw(std::runtime_error("setting server socket option `REUSEADDR` failed"));
	if (fcntl(_server_socket_fd, F_SETFL, O_NONBLOCK) == -1) // make fd nonblocking for MacOS
		throw(std::runtime_error("setting server socket to `NONBLOCK` failed"));
	if (listen(_server_socket_fd, SOMAXCONN) == -1) // wait for connections
		throw(std::runtime_error("server socket listening failed"));

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
		std::cout << "Disconnecting client " << iter->second.getFd() << std::endl;
		close(iter->second.getFd());
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
			if (iter->revents & POLLIN)
			{
				if (iter->fd == _server_socket_fd)
					acceptClient();
				else
					receiveData(iter->fd);
			}
		}
	}
	shutdown();
}

void	Server::acceptClient()
{
	Client		client;
	sockaddr_in	addr;
	socklen_t	len = sizeof(addr);

	int	client_fd = accept(_server_socket_fd, (sockaddr *)&addr, &len);
	perror("dies das irc");
	if (client_fd == -1)
		throw (std::runtime_error("accepting client failed"));

	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("setting client socket to `NONBLOCK` failed"));

	pollfd	client_poll;
	client_poll.fd = client_fd;
	client_poll.events = POLLIN;
	client_poll.revents = 0;

	client.setFd(client_fd);
	client.setIpAddr(inet_ntoa(addr.sin_addr));
	_clients[client.getNick()] = client;
	_sockets.push_back(client_poll);

	std::cout << GREEN << "Client " << client_fd << " connected" << RESET << std::endl;
}

void	Server::receiveData(int fd)
{
	char	buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);

	if (bytes <= 0)
	{
		std::cout << RED << "Client " << fd << " has disconnected" << std::endl;
		clearClient(fd);
		close(fd);
	} else {
		std::cout << MAGENTA << "Client " << fd << " data: " << RESET << buff << std::endl;
	}
}

void	Server::clearClient(int fd)
{
	for (auto iter = _sockets.begin(); iter != _sockets.end(); iter++)
		if ((*iter).fd == fd)
			_sockets.erase(iter);
	for (auto iter = _clients.begin(); iter != _clients.end(); iter++)
		if ((*iter).second.getFd() == fd)
			_clients.erase(iter);
}

// void Server::sendError(std::string numeric, std::string client, std::string msg, int fd)
// {
// 	std::string errormsg = ":" + _name + " " + numeric + " " + client + " :" + msg + "\r\n";
// 	if (send(fd, errormsg.c_str(), errormsg.size(), 0) == -1)
// 		std::cerr << "send() failed" << std::endl;
// }

// void Server::sendError(std::string numeric, std::string client, std::string channel, std::string msg, int fd)
// {
// 	std::string errormsg = ":" + _name + " " + numeric + " " + client + " " + channel + " :" + msg + "\r\n";
// 	if (send(fd, errormsg.c_str(), errormsg.size(), 0) == -1)
// 		std::cerr << "send() failed" << std::endl;
// }
template <typename... Args>
void Server::sendError(std::string numeric, int fd, std::string client, Args... args) // Pls write : infront of the msg
{
	std::stringstream ss;

	ss << ":" << _name << " " << numeric;

	((ss << args << " "), ...);
	ss <<"\r\n";
	std::string out = ss.str();
	if (send(fd, out.c_str(), out.size(), 0) == -1)
		std::cerr << "send() failed" << std::endl;
}

void Server::sendResponse(std::string message, int fd)
{
	if (send(fd, message.c_str(), message.size(), 0) == -1)
		std::cerr << "send() failed" << std::endl;
}

Client*		Server::getClient(int fd)
{
	for (auto iter = _clients.begin(); iter != _clients.end(); iter++)
		if ((*iter).second.getFd() == fd)
			return & (*iter).second;
	return NULL;
}

// forward_list<std::string>	Server::parseCommand(const std::string command)
// {
// 	forward_list<std::string>
// }

