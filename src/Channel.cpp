
#include "Channel.hpp"

Channel::Channel(const std::string &channelName) : _name(channelName)
{
}

Channel::Channel(const std::string &channelName, const std::string &key) : _name(channelName), _key(key)
{
}

Channel::~Channel()
{
}

void Channel::addUser(Client *client)
{
	if (_users.find(client) != _users.end())
		throw std::runtime_error("User is already in channel");
	if (_users.size() >= _userLimit)
		throw std::runtime_error("Channel is full");
	_users.insert(client);
	client->joinChannel(this);
	broadcastMessage(client->getNick() + " has joined the channel");
}

void Channel::removeUser(Client *client)
{
	if (_users.find(client) == _users.end())
		throw std::runtime_error("User is not in channel");
	_users.erase(client);
	client->leaveChannel(this);
	broadcastMessage(client->getNick() + " has left the channel");
	if (_operators.find(client) != _operators.end())
		_operators.erase(client);
	if (_users.empty())
		closeChannel();
}

void Channel::addOperator(Client *client)
{
	if (_users.find(client) == _users.end())
		throw std::runtime_error("Cannot make user an operator: User is not in channel");
	if (_operators.find(client) != _operators.end())
		throw std::runtime_error("User is already an operator in this channel");
	_operators.insert(client);
	client->sendMsg("You are now an operator on " + _name);
}

