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
		
		std::string _user_name;
		std::string _real_name;
		std::string _host_name;
		std::string _server_name;
		bool isAuthedBool; 

		void set_user_name(const std::string &user_name) {_user_name = user_name;}
		void set_real_name(const std::string &real_name) {_real_name = real_name;}
		void set_host_name(const std::string &hostname) {_host_name = hostname;}
		void set_server_name(const std::string &server_name) {_server_name = server_name;}
		
	public:
		CmdBuffer commandBuffer;
		bool nickname_setted;
		bool user_setted;
		Client();
		Client(int fd, const std::string &nc);
		~Client();
		int getClientFd() const;
		bool isAuthed() const{return isAuthedBool;}
		size_t get_id() const{return _id;}
		std::string get_nick() const;
		std::string get_user_name() const {return _user_name;}
		std::string get_real_name() const {return _real_name;}
		std::string get_host_name() const {return _host_name;}
		std::string get_server_name() const {return _server_name;}
		std::string get_user_info() const;
		void set_nick(const std::string &nick);
		void set_user_info(const std::string &user, const std::string &host, const std::string &server,const std::string &real);
		bool exist_nickname(const std::string &nick);
		void setisAuthed(bool b) { isAuthedBool = b;}
		void quit();
		
		bool operator<(const Client& rhs) const;
		bool operator==(const Client& rhs) const;
		bool operator!=(const Client& rhs) const;
};

std::ostream& operator<<(std::ostream& os, const Client& client);

#endif