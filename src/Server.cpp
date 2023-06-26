#include "../include/Server.hpp"
#include "../include/Client.hpp"

Server::Server(int port, std::string &password) : _port(port), _password(password), cmdManager(_clientManager, _channelManager, password) {}
Server::~Server() {}

void Server::closeAllFd(void)
{
	std::vector<Client> clients = _clientManager.getConnectClients();
	for (size_t i = 0; i < clients.size(); i++)
	{
		close(clients[i].getFd());
	}
	close(_serverSocket);
	std::cout << "All Socket Closed" << std::endl;
}

static std::string recvMessage(Client &client)
{
	//질문! 함수 내부의 static 변수는 1번만 초기화되고 이후 호출될 때마다 변하는 값을 함수 내부에서 갖고 있으려고 선언하는건데.. 왜 memset을 계속 해주는걸까?
	static char buf[BUF_LEN];
	std::memset(buf, 0, sizeof(buf));

	//recv로 읽은 결과 byte가 0일때 예외인가 정상처리인가?
	ssize_t	 byte;
	if ((byte = recv(client.getFd(), buf, sizeof(buf), 0)) < 0 || (byte > BUF_LEN))
		throw std::exception();
	
	std::cout << GREEN << "From Client " << client.getNick() << RESET << ": " << buf << std::endl;
	
	return buf;
}

void Server::receiveCommand(Client &sender, const size_t &i)
{
	try
	{
		sender.appendToRecvBuffer(recvMessage(sender));
		while (sender.hasCommand())
			cmdManager.executeCommand(sender, sender.makeCommand());
	}
	catch(const std::exception& e)
	{
		_clientManager.deleteClient(_clientManager.getClient(_pollFds[i].fd), _channelManager);
		_pollFds.erase(_pollFds.begin() + i);
	}
}

void Server::connectNewClient(size_t pollSize)
{
	int clientFd = -1;
	
	while (clientFd == -1)
	{
		if ((clientFd = accept(_serverSocket, NULL, NULL)) < 0)
			throw std::exception();
		else
		{
			if (pollSize > MAX_CLIENT)
			{
				std::cout << YELLOW << "Server is full. Please try later." << RESET << std::endl;
				close(clientFd);
			}
			else
			{
				std::cout << YELLOW << "Socket fd " << clientFd << " accepted" << RESET << std::endl;
				addNewPoll(clientFd);
				_clientManager.addClient(clientFd);
			}
		}
	};
}

void Server::addNewPoll(int socketFd)
{
	struct pollfd newPollFd;

	newPollFd.fd = socketFd;
	newPollFd.events = POLLIN | POLLOUT | POLLERR | POLLHUP;
	newPollFd.revents = 0;

	_pollFds.push_back(newPollFd);
}

void Server::PollLoop(void)
{
	while (1)
	{
		if (poll(&_pollFds[0], _pollFds.size(), TIMEOUT) == -1)
			throw std::exception();
		
		for (size_t i(0); i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents == 0)
				continue;

			if (_pollFds[i].revents & (POLLERR | POLLHUP | 32))
			{
				_clientManager.deleteClient(_clientManager.getClient(_pollFds[i].fd), _channelManager);
				_pollFds.erase(_pollFds.begin() + i);
			}
			else if (_pollFds[i].revents & POLLIN)
				(_pollFds[i].fd == _serverSocket) ? connectNewClient(_pollFds.size()) : receiveCommand(_clientManager.getClient(_pollFds[i].fd), i);
			else if (_pollFds[i].revents & POLLOUT)
			{
				_clientManager.getClient(_pollFds[i].fd).sendMessages();
			}
		}
	}
}

void Server::createServerSocket(void)
{
	if ((_serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		throw std::exception();
	
	int enable = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
		throw std::exception();
	
	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) < 0)
		throw std::exception();
	
	struct sockaddr_in serverInfo;
	std::memset(&serverInfo, 0, sizeof(serverInfo));
		
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(_port);
	
	if (bind(_serverSocket, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) < 0)
		throw std::exception();
	
	if (listen(_serverSocket, SOMAXCONN) < 0)
		throw std::exception();
	
	std::cout << YELLOW << "ft_IRC is now running.........." << RESET << std::endl;
}

void Server::run(void)
{
	createServerSocket();
	addNewPoll(_serverSocket);
	PollLoop();
}
