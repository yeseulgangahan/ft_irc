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
#include "ChannelManager.hpp"


class ClientManager
{
	std::map<int, Client> _clientMap;
public:
	void addClient(int socketfd);
	void deleteClient(Client &client, ChannelManager& channelManager);

	std::map<int, Client>::iterator find_client_by_nick(const std::string & nick);
	
	Client& getClientByFd(int fd);
	Client& get_client_by_nick(const std::string &nickname);
	
	
	const std::vector<Client> get_connect_clients();
	
	bool exist_client_by_nick(const std::string &nickname);
	
	bool require_exist_nick(Client& sender, const std::string &target_nick);
	
	void privmsg(const Command &cmd, Client &sender, const std::string &receiver_name);
	void notice(const Command &cmd, Client &sender, const std::string &receiver_name);

};


#endif // CLIENTMANAGER_HPP