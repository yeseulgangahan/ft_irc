#include "../include/Client.hpp"

static size_t generateId()
{
	static size_t curId = 0;
	return curId++;
}


Client::Client() : _id(generateId()), _fd(-1), _isAuthedBool(false), nickname_setted(false), user_setted(false) {}

Client::Client(int fd, const std::string &nc) : _id(generateId()), _fd(fd), _isAuthedBool(false), nickname_setted(false), user_setted(false) { (void)nc; }

Client::~Client() {}

int Client::getClientFd() const
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

void Client::setNick(const std::string &nick)
{
	if (IsNicknameExist(nick))
		return;
	getNicknameDict()[*this] = nick;
	nickname_setted = true;
}

void Client::setUserInfo(const std::string &user, const std::string &host, const std::string &server, const std::string &real)
{
	setUserName(user);
	setHostName(host);
	setServerName(server);
	setRealName(real);
	user_setted = true;
}

bool Client::IsNicknameExist(const std::string &nick)
{
	std::map<Client, std::string> nicks = getNicknameDict();
	for (std::map<Client, std::string>::iterator it = nicks.begin(); it != nicks.end(); it++)
		if (it->second == nick)
			return true;
	return false;
}

std::string Client::getUserInfo() const
{
	return getNick() + "!" + getUserName() + "@" + getHostName();
}

void Client::quit()
{
	getNicknameDict().erase(*this);
}

bool Client::operator<(const Client &rhs) const
{
	return _id < rhs.get_id();
}

bool Client::operator==(const Client &rhs) const
{
	return _id == rhs.get_id();
}

bool Client::operator!=(const Client &rhs) const
{
	return !operator==(rhs);
}

std::ostream &operator<<(std::ostream &os, const Client &client)
{
	os << "nick_name : " << client.getNick() << ", ";
	os << "real_name : " << client.getRealName() << ", ";
	os << "host_name : " << client.getHostName() << ", ";
	os << "client_fd : " << client.getClientFd() << std::endl;
	return os;
}
