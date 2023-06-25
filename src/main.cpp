#include "../include/Server.hpp"
#include <signal.h>
#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include "../include/ChannelManager.hpp"
#include <cstdlib>

Server *server;

void signal_handler(int signal)
{
	server->close_all_fd();
	std::cout << " exit " << std::endl;
	std::exit(signal);
}

void	checkArgv(int argc, char **argv)
{
	if (argc != 3)
		throw std::exception();
	
	int len = strlen(argv[1]);
	for (int i(0); i < len; ++i)
		if (!isdigit(argv[1][i]))
			throw std::exception();
		
	int portNum = std::atoi(argv[1]);
	if (portNum < 0 || portNum > 65535)
		throw std::exception();

	len = strlen(argv[2]);
	for (int i(0); i < len; ++i)
		if (!isgraph(argv[2][i]))
			throw std::exception();
}

int main(int argc, char **argv)
{
	//port, password 입력 안하면 segfault
	try
	{
		signal(SIGINT, signal_handler);
		//signal(SIGQUIT, signal_handler);
		//signal(SIGPIPE, SIG_IGN);
		checkArgv(argc, argv);

		int port = (int)std::atoi(argv[1]);
		std::string password(argv[2]);
		
		Server serv(port, password);
		server = &serv;
		serv.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		if (server)
			server->close_all_fd();
		return (1);
	}
	return (0);
}