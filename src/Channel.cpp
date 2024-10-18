
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
	broadcastMessage(client->getNick() + " has joined the channel");
}

void Channel::removeUser(Client *client)
{
	if (_users.find(client) == _users.end())
		throw std::runtime_error("User is not in channel");
	_users.erase(client);
	broadcastMessage(client->getNick() + " has left the channel");
	if (_operators.find(client) != _operators.end())
		_operators.erase(client);
	if (_users.empty())
		closeChannel();
}

void Channel::addOperator(Client *client)
{
	if (_users.find(client) == _users.end())
		throw std::runtime_error("Cannot make " + client->getNick() + " an operator: User is not in channel");
	if (_operators.find(client) != _operators.end())
		throw std::runtime_error("Cannot make " + client->getNick() + " an operator: User is already an operator in this channel");
	_operators.insert(client);
	client->receiveMsg("You are now an operator on " + _name);
}

void Channel::removeOperator(Client *client)
{
	if (_users.find(client) == _users.end())
		throw std::runtime_error("Cannot remove operator rights of " + client->getNick() + ": User is not in channel");
	if (_operators.find(client) == _operators.end())
		throw std::runtime_error("Cannot remove operator rights of " + client->getNick() + ": User is not an operator in this channel");
	if (_operators.size() == 1)
		throw std::runtime_error("Cannot remove operator rights of " + client->getNick() + ": User is the last operator in channel");
	_operators.erase(client);
	client->receiveMsg("You are no longer an operator on " + _name);
}
void Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

const std::string &Channel::getTopic() const
{
	return _topic;
}

void Channel::inviteUser(Client *invitedClient, Client *inviter)
{
	if (_users.find(inviter->getNick()) == _users.end())
		throw std::runtime_error("Cannot invite " + invitedClient->getNick() + ": Inviting User " + inviter->getNick() + " is not in channel");
		
}
