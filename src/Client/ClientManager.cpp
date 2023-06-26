#include "../../include/ClientManager.hpp"
#include "../../include/Message.hpp"

void ClientManager::addClient(int socketFd)
{
	Client newClient(socketFd);
	_clientMap[socketFd] = newClient;
}

Client &ClientManager::getClient(int fd)
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

Client &ClientManager::getClient(const std::string &nickname)
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
		sender.appendToSendBuffer(ERR_NOSUCHNICK(sender, targetNick));
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
	close(client.getFd());
	std::cout << YELLOW << "Total Clients: " << _clientMap.size() << RESET << std::endl;
}

void ClientManager::privmsg(const Command &cmd, Client &sender, const std::string &receiverName)
{
	if (!requireExistNick(sender, receiverName))
		return;
	getClient(receiverName).appendToSendBuffer(REP_CMD(sender, cmd));
}

void ClientManager::notice(const Command &cmd, Client &sender, const std::string &receiverName)
{
	if (!isClientExistByNick(receiverName))
		return;
	getClient(receiverName).appendToSendBuffer(REP_CMD(sender, cmd));
}