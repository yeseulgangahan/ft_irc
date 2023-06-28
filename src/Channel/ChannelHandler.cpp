#include "../../include/ChannelHandler.hpp"

channelIter ChannelHandler::getChannelIterator(const std::string& channelName) const
{
	for(channelIter it = _channels.begin(); it != _channels.end(); it++)
		if (it->getName() == channelName)
			return it;
	return _channels.end();
}

bool ChannelHandler::isChannelExist(const std::string& channelName) const
{
	return getChannelIterator(channelName) != _channels.end();
}

Channel& ChannelHandler::getChannel(const std::string& channelName) const
{
	channelIter ch = getChannelIterator(channelName);
	if (ch == _channels.end())
		throw std::logic_error("must not use ChanndlHandler::find_must_exist(channelName) when not exist");
	return const_cast<Channel&>(*ch);
}

std::set<Channel> ChannelHandler::getChannels() const 
{
	return _channels;
}

//addClient에 해당하는 채널매니저의 동작: add participant
void ChannelHandler::addClient(const Command& cmd, Client &client, const std::vector<std::string> &channelList, const std::vector<std::string> &ch_pass)
{
	for(size_t i = 0; i < channelList.size(); i++)
	{
		if (requireValidChannelName(client, channelList[i]))//채널이름 규칙에 맞는지
		{
            if (isChannelExist(channelList[i]))//존재하는 채널이면
                getChannel(channelList[i]).addMember(cmd, client, ch_pass[i]);//그 채널에 유저 추가
            else
                _channels.insert(Channel(cmd, channelList[i], client, ch_pass[i]));//새로운 채널을 생성해서 넣는다
        }
	}
}

void ChannelHandler::removeTerminatedClient(Client &client)
{
	for(channelIter channelIter = _channels.begin(); channelIter != _channels.end(); channelIter++)
	{
		Channel &channel = const_cast<Channel&>(*channelIter);

		if (channel.isMember(client))//체널의 멤버면 멤버를 지운다
		{
			channel.removeMember(client);
			if (channel.getMembers().empty())//채널이 텅 비었으면 채널도 지운다
				getChannels().erase(channel);
		}
	}
}

void ChannelHandler::removeDepartedClient(const Command& cmd, Client &sender, std::vector<std::string> &channelList)
{
	for(size_t i = 0; i < channelList.size(); i++)
	{
		if (requireExistChannel(sender, channelList[i]))//존재하는 채널이면
		{
			getChannel(channelList[i]).broadcast(sender, REPLY(sender.getUserString(), cmd._commandName, channelList[i], ""));//해당 채널에서 PART 메시지를 보내야 한다
			getChannel(channelList[i]).removeMemberCheckOperator(sender, channelList[i]);//떠난다

		}
	}
}

bool ChannelHandler::requireValidChannelName(Client &client, const std::string & channel_name)
{
	if ((channel_name == "" || channel_name[0] != '#' || channel_name == "#"))
	{
		client.appendToSendBuffer(ERR_NOSUCHCHANNEL(client, channel_name));
		return false;
	}
	return true;
}

bool ChannelHandler::requireExistChannel(Client &client, const std::string & channel_name)
{
	if (!isChannelExist(channel_name))
	{
		client.appendToSendBuffer(ERR_NOSUCHCHANNEL(client, channel_name));
		return false;
	}
	return true;
}

void ChannelHandler::broadcastToChannel(const Command &cmd, Client &sender, const std::string &channel_name)
{
	if (!requireExistChannel(sender, channel_name))
		return;
	getChannel(channel_name).broadcastExceptSender(sender,  REP_CMD(sender, cmd));
}

// NICK, QUIT에서 사용된다
void ChannelHandler::replyToAllPeerClients(Client &sender, const Command &cmd)// 같은 채널에 속한 사람들에게 다 알려주기 (나 빼고)
{
	std::vector<int> peerClients;//같은 클라이언트에게 여러 번 보내지 않도록 한 번 보내면 여기에 저장

	for(channelIter channelIter = _channels.begin(); channelIter != _channels.end(); channelIter++)
	{
		if ((*channelIter).isMember(sender))//내가 속한 채널이다!
		{
			//클라이언트 돌아가면서
			const std::set<Client> members = (*channelIter).getMembers();
			for (clientIter clientIter = members.begin(); clientIter != members.end(); clientIter++)
			{
				if ((std::find(peerClients.begin(), peerClients.end(), clientIter->getFd()) == peerClients.end()) && //아직 peer client에 저장 안했다
				(*clientIter) != sender)//내가 아닌 거면
				{
					//벡터에 기록하고 샌드
					peerClients.push_back(clientIter->getFd());
					std::string message = REP_CMD(sender, cmd);
					message += "\r\n";
					send(clientIter->getFd(), message.c_str(), message.length(), 0);
					std::cout << BLUE << "To Client " << clientIter->getNick() << " " << RESET << message <<std::endl;
				}
			}
		}
	}
}

