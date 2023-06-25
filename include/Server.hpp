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
# include "ChannelManager.hpp"
# include "ClientManager.hpp"
# include "CmdManager.hpp"

# define TIMEOUT 5000

# define BUF_LEN 4096

class Client;
class Server
{
	private:
		int 						_port;
		std::string					_password;
		ChannelManager				_channelManager;
		ClientManager				_clientManager;
		std::deque<struct pollfd>	_pollFds;
		int							_serverSocket;
		
		void addNewPoll(int socket_fd);
		void set_signal(void);

	public:
		CmdManager		cmdManager;
		
		Server(int port, std::string &password);
		~Server();
		
		void	run(void);
		void	close_all_fd(void);
		void	createServerSocket(void);
		void	addClient(int socketfd);
		void	receiveCommand(Client &client);
		void	PollLoop(void);
		void	connectNewClient(void);
		void	build_in(Client &sender, const Command &cmd);
};

#endif