#include "ClientManager.hpp"
#include "Message.hpp"

void ClientManager::addClient(int socketFd)
{
	const std::string tmpNick = "unknown" + std::to_string(socketFd);
	Client newClient(socketFd, tmpNick);
	
	_clientMap[socketFd] = newClient;
}

Client &ClientManager::getClientByFd(int fd)
{
	return _clientMap[fd];
}

std::map<int, Client>::iterator ClientManager::find_client_by_nick(const std::string &nick)
{
	for (std::map<int, Client>::iterator it = _clientMap.begin(); it != _clientMap.end(); it++)
		if (it->second.get_nick() == nick)
			return it;
	return _clientMap.end();
}

bool ClientManager::exist_client_by_nick(const std::string &nickname)
{
	return find_client_by_nick(nickname) != _clientMap.end();
}

Client &ClientManager::get_client_by_nick(const std::string &nickname)
{
	assert(exist_client_by_nick(nickname));
	return const_cast<Client &>(find_client_by_nick(nickname)->second);
}

const std::vector<Client> ClientManager::get_connect_clients()
{
	std::vector<Client> connect_clients;
	for (std::map<int, Client>::iterator it = _clientMap.begin(); it != _clientMap.end(); it++)
		connect_clients.push_back(it->second);
	return connect_clients;
}

bool ClientManager::require_exist_nick(Client &sender, const std::string &target_nick)
{
	if (!exist_client_by_nick(target_nick))
	{
		reply(sender, ERR_NOSUCHNICK(sender, target_nick));
		return false;
	}
	return true;
}

void ClientManager::deleteClient(Client &client, ChannelManager &channelManager)
{
	std::cout << YELLOW << "Client " << client.get_nick() << " disconnected" << RESET << std::endl;
	channelManager.remove_by_all_channel(client);
	assert(exist_client_by_nick(client.get_nick()));
	_clientMap.erase(find_client_by_nick(client.get_nick()));
	client.quit();
	close(client.getClientFd());
	std::cout << YELLOW << "Total Clients: " << _clientMap.size() << RESET << std::endl;
}

void ClientManager::privmsg(const Command &cmd, Client &sender, const std::string &receiver_name)
{
	if (!require_exist_nick(sender, receiver_name))
		return;
	Client receiver = get_client_by_nick(receiver_name);
	reply(receiver, REP_CMD(sender, cmd));
}

void ClientManager::notice(const Command &cmd, Client &sender, const std::string &receiver_name)
{
	if (!exist_client_by_nick(receiver_name))
		return;
	Client receiver = get_client_by_nick(receiver_name);
	reply(receiver, REP_CMD(sender, cmd));
}