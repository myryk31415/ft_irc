
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
	if (_users.find(client->getNick()) != _users.end())
		throw std::runtime_error("User is already in channel");
	if (_users.size() >= _userLimit)
		throw std::runtime_error("Channel is full");
	_users[client->getNick()] = client;
	broadcastMessage(client->getNick() + " has joined the channel");
}

void Channel::removeUser(Client *client)
{
	if (_users.find(client->getNick()) == _users.end())
		throw std::runtime_error("User is not in channel");
	_users.erase(client->getNick());
	broadcastMessage(client->getNick() + " has left the channel");
	if (_operators.find(client->getNick()) != _operators.end())
		_operators.erase(client->getNick());
	if (_users.empty())
		closeChannel();
}

void Channel::addOperator(Client *client)
{
	if (_users.find(client->getNick()) == _users.end())
		throw std::runtime_error("Cannot make " + client->getNick() + " an operator: User is not in channel");
	if (_operators.find(client->getNick()) != _operators.end())
		throw std::runtime_error("Cannot make " + client->getNick() + " an operator: User is already an operator in this channel");
	_operators[client->getNick()] = client;
	client->receiveMsg("You are now an operator on " + _name);
}

void Channel::removeOperator(Client *client)
{
	if (_users.find(client->getNick()) == _users.end())
		throw std::runtime_error("Cannot remove operator rights of " + client->getNick() + ": User is not in channel");
	if (_operators.find(client->getNick()) == _operators.end())
		throw std::runtime_error("Cannot remove operator rights of " + client->getNick() + ": User is not an operator in this channel");
	if (_operators.size() == 1)
		throw std::runtime_error("Cannot remove operator rights of " + client->getNick() + ": User is the last operator in channel");
	_operators.erase(client->getNick());
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
	if (this->isModeSet(INVITE_ONLY) && _operators.find(inviter->getNick()) == _operators.end())
		throw std::runtime_error("Cannot invite " + invitedClient->getNick() + ": Channel is set to invite-only, but Inviting User " + inviter->getNick() + " is not an operator");
	if (_users.find(invitedClient->getNick()) != _users.end())
		throw std::runtime_error("Cannot invite " + invitedClient->getNick() + ": User is already in channel");
	if (_invitedUsers.find(invitedClient->getNick()) != _invitedUsers.end())
		throw std::runtime_error("Cannot invite " + invitedClient->getNick() + ": User is already invited");
	_invitedUsers[invitedClient->getNick()] = invitedClient;
	invitedClient->receiveMsg("You have been invited to the channel " + this->_name + " by " + inviter->getNick());
}

void Channel::setMode(int mode)
{
	if (_modes[mode] == 1)
		throw std::runtime_error("Mode already set");
	_modes[mode] = 1;
}

void Channel::unsetMode(int mode)
{
	if (_modes[mode] == 0)
		throw std::runtime_error("Mode not set");
	_modes[mode] = 0;
}

bool Channel::isModeSet(int mode) const
{
	return (_modes[mode]);
}
