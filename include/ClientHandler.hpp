#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <iostream>
#include <algorithm>
#include <unistd.h> 
#include <string> 
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "Client.hpp"
#include "Command.hpp"
#include "ChannelHandler.hpp"


class ClientHandler
{
	std::map<int, Client> _clientMap;

public:
	void addClient(int socketfd);
	void deleteClient(Client &client, ChannelHandler& channelHandler);

	std::map<int, Client>::iterator findClientByNick(const std::string & nick);

	Client& getClient(int fd);
	Client& getClient(const std::string &nickname);
	
	const std::vector<Client> getConnectClients();
	
	bool isClientExistByNick(const std::string &nickname);
	
	bool requireExistNick(Client& sender, const std::string &targetNick);
	
	void privmsg(const Command &cmd, Client &sender, const std::string &receiver_name);
	void notice(const Command &cmd, Client &sender, const std::string &receiver_name);

};


#endif // CLIENTMANAGER_HPP