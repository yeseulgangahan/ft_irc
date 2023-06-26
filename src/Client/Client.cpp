#include "../../include/Client.hpp"

static size_t generateId()
{
	static size_t curId = 0;
	return curId++;
}

Client::Client() {}

Client::Client(int fd, const std::string &nc):
	_id(generateId()), 
	_fd(fd), 
	_isAuthed(false), 
	_isNicknameSetted(false), 
	_isUserSetted(false),
	_isRegistrationDone(false)
{ (void)nc; }

Client::~Client() {}

int Client::getFd() const
{
	return (_fd);
}

static std::map<Client, std::string> &getNicknameDict()
{
	static std::map<Client, std::string> nickname_dict;
	return nickname_dict;
}

std::string Client::getNick() const
{
	return getNicknameDict()[*this];
}

std::string Client::getUserName() const
{
	return _userName;
}

std::string Client::getRealName() const
{
	return _realName;
}

std::string Client::getHostName() const
{
	return _hostName;
}


void Client::setNick(const std::string &nick)
{
	if (isNicknameExist(nick))
		return;
	getNicknameDict()[*this] = nick;
	_isNicknameSetted = true;
}

void Client::setUserInfo(const std::string &user, const std::string &real, const std::string &host, const std::string &server)
{
	_userName = user;
	_realName = real;
	_hostName = host;
	_serverName = server;
	_isUserSetted = true;
}

void Client::setIsAuthed() { _isAuthed = true; }

void Client::setIsRegistrationDone() { _isRegistrationDone = true; }

bool Client::isAuthed() { return _isAuthed; } 

bool Client::isNicknameSetted() { return _isNicknameSetted; }

bool Client::isUserSetted() { return _isUserSetted; }

bool Client::isRegistrationDone() { return _isRegistrationDone; }

bool Client::isNicknameExist(const std::string &nick)
{
	std::map<Client, std::string> nicks = getNicknameDict();
	for (std::map<Client, std::string>::iterator it = nicks.begin(); it != nicks.end(); it++)
		if (it->second == nick)
			return true;
	return false;
}

std::string Client::getUserString() const
{
	return getNick() + "!" + _userName + "@" + _hostName;
}

void Client::quit()
{
	getNicknameDict().erase(*this);
}

bool Client::operator<(const Client &rhs) const
{
	return _id < rhs._id;
}

bool Client::operator==(const Client &rhs) const
{
	return _id == rhs._id;
}

bool Client::operator!=(const Client &rhs) const
{
	return !operator==(rhs);
}
