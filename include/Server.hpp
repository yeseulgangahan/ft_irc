#ifndef SERVER_HPP
# define SERVER_HPP 

# include <iostream>
# include <algorithm>
# include <sys/socket.h> 
# include <sys/types.h>
# include <arpa/inet.h>
# include <unistd.h> 
# include <string> 
# include <fcntl.h>
# include <signal.h>
# include <sys/poll.h>
# include <deque>
# include <string>
# include <map>

# include "Message.hpp"
# include "ChannelHandler.hpp"
# include "ClientHandler.hpp"
# include "CmdHandler.hpp"

# define TIMEOUT 5000
# define BUF_LEN 4096
# define MAX_CLIENT 3

class Client;
class Server
{
	private:
		int 						_port;
		std::string					_password;
		ChannelHandler				_channelHandler;
		ClientHandler				_clientHandler;
		std::deque<struct pollfd>	_pollFds;
		int							_serverSocket;
		
		void addNewPoll(int socket_fd);

	public:
		CmdHandler		cmdHandler;
		
		Server(int port, std::string &password);
		~Server();
		
		void	run(void);
		void	closeAllFd(void);
		void	createServerSocket(void);
		void	addClient(int socketfd);
		bool	receiveCommand(Client &client, const size_t &i);
		void	PollLoop(void);
		bool	connectNewClient(size_t pollSize);
};

#endif
