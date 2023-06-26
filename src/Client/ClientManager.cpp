#include "../../include/ClientManager.hpp"
#include "../../include/Message.hpp"

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

std::map<int, Client>::iterator ClientManager::findClientByNick(const std::string &nick)
{
	for (std::map<int, Client>::iterator it = _clientMap.begin(); it != _clientMap.end(); it++)
		if (it->second.getNick() == nick)
			return it;
	return _clientMap.end();
}

bool ClientManager::isClientExistByNick(const std::string &nickname)
{
	return findClientByNick(nickname) != _clientMap.end();
}

Client &ClientManager::getClientByNick(const std::string &nickname)
{
	assert(isClientExistByNick(nickname));
	return const_cast<Client &>(findClientByNick(nickname)->second);
}

const std::vector<Client> ClientManager::getConnectClients()
{
	std::vector<Client> connect_clients;
	for (std::map<int, Client>::iterator it = _clientMap.begin(); it != _clientMap.end(); it++)
		connect_clients.push_back(it->second);
	return connect_clients;
}

bool ClientManager::requireExistNick(Client &sender, const std::string &targetNick)
{
	if (!isClientExistByNick(targetNick))
	{
		reply(sender, ERR_NOSUCHNICK(sender, targetNick));
		return false;
	}
	return true;
}

void ClientManager::deleteClient(Client &client, ChannelManager &channelManager)
{
	std::cout << YELLOW << "Client " << client.getNick() << " disconnected" << RESET << std::endl;
	channelManager.removeTerminatedClient(client);
	assert(isClientExistByNick(client.getNick()));
	_clientMap.erase(findClientByNick(client.getNick()));
	client.quit();
	close(client.getClientFd());
	std::cout << YELLOW << "Total Clients: " << _clientMap.size() << RESET << std::endl;
}

void ClientManager::privmsg(const Command &cmd, Client &sender, const std::string &receiver_name)
{
	if (!requireExistNick(sender, receiver_name))
		return;
	Client receiver = getClientByNick(receiver_name);
	reply(receiver, REP_CMD(sender, cmd));
}

void ClientManager::notice(const Command &cmd, Client &sender, const std::string &receiver_name)
{
	if (!isClientExistByNick(receiver_name))
		return;
	Client receiver = getClientByNick(receiver_name);
	reply(receiver, REP_CMD(sender, cmd));
}