#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <algorithm>
#include <unistd.h> 
#include <string> 
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cassert>
#include <cstring>
#include "Command.hpp"

# define RESET "\033[0m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define YELLOW "\033[0;33m"


class Client
{
	private:
		int		_fd;
		
		std::string _userName;
		std::string _realName;
		std::string _hostName;
		std::string _serverName;
		
		bool _isAuthed;
		bool _isNicknameSetted;
		bool _isUserSetted;
		bool _isRegistrationDone;
	
		std::string _recvBuffer;
		std::string _sendBuffer;

	public:
		int getFd() const;
		std::string getUserName() const;
		std::string getRealName() const;
		std::string getHostName() const;
		std::string getNick() const;
		std::string getUserString() const;
		
		void setNick(const std::string &nick);
		void setUserInfo(const std::string &user, const std::string &real, const std::string &host, const std::string &server);
		void setIsAuthed();
		void setIsRegistrationDone();

		bool isAuthed();
		bool isNicknameSetted();
		bool isUserSetted();
		bool isRegistrationDone();
		bool isNicknameExist(const std::string &nick);

		// recvBuffer 관련
		void appendToRecvBuffer(const std::string &s);
		bool hasCommand();
		Command makeCommand();

		//sendBuffer 관련
		void appendToSendBuffer(const std::string &s);
		void sendMessages();
		
		Client();
		Client(int fd);
		~Client();
		
		void quit();
		
		bool operator<(const Client& rhs) const;
		bool operator==(const Client& rhs) const;
		bool operator!=(const Client& rhs) const;
};

#endif