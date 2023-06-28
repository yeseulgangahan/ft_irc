#include "../../include/Client.hpp"
#include "../../include/CmdHandler.hpp"

Client::Client() {}

Client::Client(int fd)
:	
	_fd(fd), 
	_isAuthed(false), 
	_isNicknameSetted(false), 
	_isUserSetted(false),
	_isRegistrationDone(false)
{}

Client::~Client() {}

int Client::getFd() const
{
	return (_fd);
}

static std::map<Client, std::string> &getNicknameDict()
{
	static std::map<Client, std::string> nicknameDict;//여기에 닉네임이 저장되어 있다.
	return nicknameDict;
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

static std::string cutFrontSpace(std::string s)
{
	while (s.size() > 0 && s[0] == ' ')
		s.erase(s.begin());
	return s;
}

static std::string cutFrontCRLF(std::string s)
{
	while (s.length())
	{
		if (s[0] == ' ' || s[0] == '\n')
			s.erase(s.begin());
		else if (s.length() >= 2 && !s.compare(0, 2, "\r\n"))
			s.erase(s.begin(), s.begin() + 2);
		else
			break;
	}

	return (s);
}
//:<source> command parameters
static std::string trimSource(std::string s)
{
	s = cutFrontSpace(s);
	
	if (s.length() == 0)
		return (s);

	if (s[0] == ':')
	{
		size_t spacePos = s.find(' ');
		if (spacePos == std::string::npos)
			return ("");
		
		size_t commandBegin = spacePos;
		while (commandBegin < s.size() && s[commandBegin] == ' ')
			commandBegin++;

		return (s.substr(commandBegin));
	}

	return (s);
}

void Client::appendToRecvBuffer(const std::string &s)
{
	_recvBuffer += s;
	_recvBuffer = cutFrontCRLF(_recvBuffer);
}

bool Client::hasCommand()
{
	return (_recvBuffer.find("\n") != std::string::npos);
}

Command Client::makeCommand()
{
	size_t curCommandEnd;
	size_t nextCommandStart;

	if ((curCommandEnd = _recvBuffer.find("\r\n")) != std::string::npos)
		nextCommandStart = curCommandEnd + 2;
	else if ((curCommandEnd = _recvBuffer.find("\n")) != std::string::npos)
		nextCommandStart = curCommandEnd + 1;
	else
		throw std::runtime_error("Error: in commandBuffer");

	std::string wholeString = trimSource(_recvBuffer.substr(0, curCommandEnd));
	_recvBuffer = cutFrontCRLF(_recvBuffer.substr(nextCommandStart));

	return Command(wholeString);
}

void Client::appendToSendBuffer(const std::string &s)
{
	_sendBuffer += s;
	_sendBuffer += "\r\n";
}

void Client::sendMessages()
{
	send(_fd, _sendBuffer.c_str(), _sendBuffer.length(), 0);
	if (!_sendBuffer.empty())
	{
		std::cout << BLUE << "To Client " << getNick() << " " << RESET << _sendBuffer <<std::endl;
		_sendBuffer.clear();
	}
}

void Client::quit()
{
	getNicknameDict().erase(*this);
}

bool Client::operator<(const Client &rhs) const
{
	return _fd < rhs._fd;
}

bool Client::operator==(const Client &rhs) const
{
	return _fd == rhs._fd;
}

bool Client::operator!=(const Client &rhs) const
{
	return !operator==(rhs);
}
