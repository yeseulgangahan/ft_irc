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
#include "CmdBuffer.hpp"

# define RESET "\033[0m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define YELLOW "\033[0;33m"


class Client
{
	private:
		size_t	_id; 
		int		_fd;
		
		std::string _userName;
		std::string _realName;
		std::string _hostName;
		std::string _serverName;
		
		bool _isAuthed;
		bool _isNicknameSetted;
		bool _isUserSetted;
		bool _isRegistrationDone;
	
	public:
		CmdBuffer commandBuffer;

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
		
		Client();// 언제 사용되지?
		Client(int fd, const std::string &nc);//nc 뭐지?
		~Client();
		
		void quit();
		
		bool operator<(const Client& rhs) const;
		bool operator==(const Client& rhs) const;
		bool operator!=(const Client& rhs) const;
};

#endif