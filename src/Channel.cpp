
#include "Channel.hpp"

Channel::Channel(const std::string &channelName) : _name(channelName), _modes(5)
{
}

Channel::Channel(const std::string &channelName, const std::string &key) : _name(channelName), _modes(5)
{
	setMode(KEY, key, true);
}

Channel::~Channel()
{
}

void Channel::addUser(Client &client)
{
	if (_users.find(client.getNick()) != _users.end())
		throw std::runtime_error("User is already in channel");
	if (isModeSet(USER_LIMIT) && std::atoi(_modes[USER_LIMIT].second.c_str()) <= _users.size())
		throw std::runtime_error("Channel is full");
	_users[client.getNick()] = client;
	systemMessage(client.getNick() + " has joined the channel");
}

void Channel::removeUser(Client &client)
{
	if (_users.find(client.getNick()) == _users.end())
		throw std::runtime_error("User is not in channel");
	_users.erase(client.getNick());
	systemMessage(client.getNick() + " has left the channel");
	if (_operators.find(client.getNick()) != _operators.end())
		_operators.erase(client.getNick());
}

void Channel::addOperator(Client &client)
{
	if (_operators.find(client.getNick()) != _operators.end())
		{std::cerr << "Cannot make " + client.getNick() + " an operator: User is already an operator in this channel" << std::endl; return ;}
	_operators[client.getNick()] = client;
	client.receiveMsg("You are now an operator on " + _name);
}

void Channel::removeOperator(Client &client)
{
	if (_operators.find(client.getNick()) == _operators.end())
		{std::cerr << "Cannot remove operator rights of " + client.getNick() + ": User is not an operator in this channel" << std::endl; return ;}
	if (_operators.size() == 1)
		{std::cerr << "Cannot remove operator rights of " + client.getNick() + ": User is the last operator in channel" << std::endl; return ;} //sollen wir ?
	_operators.erase(client.getNick());
	client.receiveMsg("You are no longer an operator on " + _name);
}
void Channel::setTopic(const std::pair<std::string, std::string> &topic)
{
	_topic = topic;
}

const std::pair<std::string, std::string> &Channel::getTopic() const
{
	return _topic;
}


void Channel::inviteUser(Client &invitedClient, Client &inviter)
{
	_invitedUsers[invitedClient.getNick()] = invitedClient;
}

void Channel::setMode(int mode, std::string value, bool set)
{
	if (mode >= 0 && mode < static_cast<int>(_modes.size()))
	{
		_modes[mode].first = set;
		_modes[mode].second = value;
	}
}

bool Channel::isModeSet(int mode) const
{
	return (_modes[mode].first);
}

bool Channel::isUserInvited(Client &client) const
{
	if (_invitedUsers.find(client.getNick()) != _invitedUsers.end())
		return true;
	return false;
}

void Channel::systemMessage(const std::string &message)
{
	std::for_each(_users.begin(), _users.end(), [message](auto pair) {
		pair.second.receiveMsg(message);
	});
}

void Channel::broadcastMessage(const std::string &message, Client &sender)
{
	std::string newMessage = "<" + sender.getNick() + "> " + message;
	if (_users.size() == 1)
		return ;
	if (_users.size() == 2)
	{
		if (_users.begin()->second.getNick() != sender.getNick())
			_users.begin()->second.receiveMsg(newMessage);
		else
			(++_users.begin())->second.receiveMsg(newMessage);
		return ;
	}
	for (auto it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second.getNick() != sender.getNick())
			it->second.receiveMsg(newMessage);
	}
}

const std::string& Channel::getName() const
{
	return _name;
}

Client *Channel::getUser(std::string user)
{
	auto it = _users.find(user);
	if (it == _users.end())
		return NULL;
	return (&it->second);
}

Client *Channel::getOperator(std::string operatr)
{
	auto it = _operators.find(operatr);
	if (it == _operators.end())
		return NULL;
	return (&it->second);
}

std::string Channel::getModestring() const
{
	std::string modestring = "+";

	if (isModeSet(INVITE_ONLY))
		modestring += "i";
	if (isModeSet(TOPIC_SETTABLE_BY_OPERATOR))
		modestring += "t";
	if (isModeSet(MODERATED))
		modestring += "o";
	if (isModeSet(USER_LIMIT))
		modestring += "l";
	if (isModeSet(KEY))
		modestring += "k";
	if (modestring.size() == 1)
		return "";
	return modestring;
}


std::string Channel::getModesvalues() const
{
	std::string modestring;

	if (isModeSet(USER_LIMIT))
		modestring += _modes[USER_LIMIT].second;
	if (isModeSet(KEY))
		modestring += _modes[KEY].second;
	return modestring;
}


int Channel::getUserCount() const
{
	return _users.size();
}
