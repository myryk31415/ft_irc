
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
	for (auto it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << "Disconnecting client " << it->first << std::endl;
		close(it->first);
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
		for (size_t i = 0; i < _sockets.size(); ++i)
		{
			if (_sockets[i].revents & POLLIN)
			{
				if (_sockets[i].fd == _server_socket_fd)
					acceptClient();
				else
					receiveData(_sockets[i].fd);
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
	if (client_fd == -1)
		throw (std::runtime_error("accepting client failed"));

	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("setting client socket to `NONBLOCK` failed"));

	pollfd	client_poll;
	client_poll.fd = client_fd;
	client_poll.events = POLLIN;
	client_poll.revents = 0;

	client.setFd(client_fd);
	client.setNick(inet_ntoa(addr.sin_addr));
	client.setIpAddr(client.getNick());
	_clients[client_fd] = client;
	_sockets.push_back(client_poll);

	std::cout << GREEN << "Client " << client_fd << " connected" << RESET << std::endl;
}

void splitData(std::string data, std::vector<std::string> &cmd)
{
	std::istringstream	strm(data);
	std::string			line;
	while (std::getline(strm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		cmd.push_back(line);
	}
}

void	Server::receiveData(int fd)
{
	char	buff[1024];
	std::vector<std::string> cmd;
	// memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);
	buff[bytes] = 0;
	if (bytes <= 0)
	{
		std::cout << RED << "Client " << fd << " has disconnected" << std::endl;
		clearClient(fd);
		close(fd);
	} else {
		std::cout << MAGENTA << "Client " << fd << " data: " << RESET << buff << std::endl;
		splitData(buff, cmd);
		if (cmd[0].compare("CAP LS 302"))
			sendResponse("CAP * LS :\r\n", fd);
		if (cmd[0].compare("CAP REQ :"))
			sendResponse("CAP * ACK :\r\n", fd);
		if (cmd[0].compare("CAP END"))
			sendResponse(": 001 MONI DIGGA", fd);
		for (auto it = cmd.begin(); it != cmd.end(); it++)
			parseCommand(*it, fd);
	}
}

void	Server::clearClient(int fd)
{
	close(fd);
	for (auto it = _sockets.begin(); it != _sockets.end();)
		if (it->fd == fd)
			it = _sockets.erase(it);
		else
			it++;
	_clients.erase(fd);
}

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

Client*	Server::getClient(int fd)
{
	if (_clients.find(fd) == _clients.end())
		return NULL;
	return &_clients[fd];
}

Client*	Server::getClient(std::string nick)
{
	for (auto iter = _clients.begin(); iter != _clients.end(); iter++)
		if ((*iter).second.getNick() == nick)
			return & (*iter).second;
	return NULL;
}

void	Server::parseCommand(const std::string command, int fd)
{
	std::string	cmd;
	std::vector<std::string> args;
	size_t	cmd_end = command.find_first_of(' ');
	size_t	colon = command.find_first_of(':');

	if (cmd_end == std::string::npos)
		cmd_end = command.size();
	cmd_end = std::min(colon, cmd_end);
	cmd = command.substr(0, cmd_end);
	args = parseArgs(command.substr(cmd_end), fd);

	std::cout << std::endl << "PARSED COMMAND:" << std::endl;
	std::cout << '"' << cmd << '"' << std::endl;
	std::cout << std::endl << "ARGS:" << std::endl;
	for (auto it = args.begin(); it != args.end(); it++)
		std::cout << '"' << *it << '"' << std::endl;
	std::cout << std::endl;
	// cmdDecide(cmd, args, fd);
}

std::vector<std::string>	Server::parseArgs(const std::string command_args, int fd)
{
	std::vector<std::string>	args;
	size_t	colon;
	std::stringstream obj(command_args);
	std::string	temp;

	while (obj >> temp)
	{
		colon = temp.find_first_of(':');
		if (colon != std::string::npos)
		{
			if (colon > 0)
				args.emplace_back(temp.substr(0, colon));
			break;
		}
		args.emplace_back(temp);
	}
	colon = command_args.find_first_of(':');
	if (colon != std::string::npos)
		args.emplace_back(command_args.substr(colon + 1));
	return args;
}
#define CMD_PAIR(cmd) commands.push_back(std::make_pair(#cmd, cmd));

void	Server::cmdDecide(const std::string cmd, const std::vector<std::string> args, int fd)
{
	std::vector<std::pair<std::string, void (Server::*)(std::vector<std::string>, int)>>	commands;
	CMD_PAIR(MODE);

	for (auto it = commands.begin(); it != commands.end(); it++)
		if (!it->first.compare(cmd))
			(this->*(it->second))(args, fd);
}

void	Server::finishRegistration(int fd)
{
	std::string	msg;
	Client		client = _clients[fd];

	msg.append(client.getUsername()).append(" :Welcome to the ").append(_name).append(", ").append(client.getNick()); //todo [!<user>@<host>]
	sendResponse(msg, fd);
	msg.clear();
	msg.append(client.getUsername()).append(" :Your host is ").append(_name).append(", running version ").append("version");
	sendResponse("RPL_YOURHOST\r\n", fd);
	sendResponse("RPL_CREATED\r\n", fd);
	sendResponse("RPL_MYINFO\r\n", fd);
}
