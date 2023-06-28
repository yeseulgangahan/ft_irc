#include "../../include/ClientHandler.hpp"
#include "../../include/Message.hpp"

void ClientHandler::addClient(int socketFd)
{
	Client newClient(socketFd);
	_clientMap[socketFd] = newClient;
}

Client &ClientHandler::getClient(int fd)
{
	return _clientMap[fd];
}

std::map<int, Client>::iterator ClientHandler::findClientByNick(const std::string &nick)
{
	for (std::map<int, Client>::iterator it = _clientMap.begin(); it != _clientMap.end(); it++)
		if (it->second.getNick() == nick)
			return it;
	return _clientMap.end();
}

bool ClientHandler::isClientExistByNick(const std::string &nickname)
{
	return findClientByNick(nickname) != _clientMap.end();
}

Client &ClientHandler::getClient(const std::string &nickname)
{
	assert(isClientExistByNick(nickname));
	return const_cast<Client &>(findClientByNick(nickname)->second);
}

const std::vector<Client> ClientHandler::getConnectClients()
{
	std::vector<Client> connect_clients;
	for (std::map<int, Client>::iterator it = _clientMap.begin(); it != _clientMap.end(); it++)
		connect_clients.push_back(it->second);
	return connect_clients;
}

bool ClientHandler::requireExistNick(Client &sender, const std::string &targetNick)
{
	if (!isClientExistByNick(targetNick))
	{
		sender.appendToSendBuffer(ERR_NOSUCHNICK(sender, targetNick));
		return false;
	}
	return true;
}

void ClientHandler::deleteClient(Client &client, ChannelHandler &channelHandler)
{
	std::cout << YELLOW << "Client " << client.getNick() << " disconnected" << RESET << std::endl;
	channelHandler.removeTerminatedClient(client);
	assert(isClientExistByNick(client.getNick()));
	_clientMap.erase(findClientByNick(client.getNick()));
	client.quit();
	close(client.getFd());
	std::cout << YELLOW << "Total Clients: " << _clientMap.size() << RESET << std::endl;
}

void ClientHandler::privmsg(const Command &cmd, Client &sender, const std::string &receiverName)
{
	if (!requireExistNick(sender, receiverName))
		return;
	getClient(receiverName).appendToSendBuffer(REP_CMD(sender, cmd));
}

void ClientHandler::notice(const Command &cmd, Client &sender, const std::string &receiverName)
{
	if (!isClientExistByNick(receiverName))
		return;
	getClient(receiverName).appendToSendBuffer(REP_CMD(sender, cmd));
}