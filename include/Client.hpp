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
		bool _isAuthedBool; 

		void setUserName(const std::string &user_name) {_userName = user_name;}
		void setRealName(const std::string &real_name) {_realName = real_name;}
		void setHostName(const std::string &hostname) {_hostName = hostname;}
		void setServerName(const std::string &server_name) {_serverName = server_name;}
		
	public:
		CmdBuffer commandBuffer;
		bool nickname_setted;
		bool user_setted;
		Client();
		Client(int fd, const std::string &nc);
		~Client();
		int getClientFd() const;
		bool isAuthed() const{return _isAuthedBool;}
		size_t get_id() const{return _id;}
		std::string getNick() const;
		std::string getUserName() const {return _userName;}
		std::string getRealName() const {return _realName;}
		std::string getHostName() const {return _hostName;}
		std::string getServerName() const {return _serverName;}
		std::string getUserInfo() const;
		void setNick(const std::string &nick);
		void setUserInfo(const std::string &user, const std::string &host, const std::string &server,const std::string &real);
		bool IsNicknameExist(const std::string &nick);
		void setisAuthed(bool b) { _isAuthedBool = b;}
		void quit();
		
		bool operator<(const Client& rhs) const;
		bool operator==(const Client& rhs) const;
		bool operator!=(const Client& rhs) const;
};

std::ostream& operator<<(std::ostream& os, const Client& client);

#endif