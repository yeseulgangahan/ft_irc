#include "../../include/ChannelHandler.hpp"

channelIter ChannelHandler::getChannelIterator(const std::string& channelName) const
{
	for(channelIter it = channels.begin(); it != channels.end(); it++)
		if (it->getName() == channelName)
			return it;
	return channels.end();
}

bool ChannelHandler::isChannelExist(const std::string& channelName) const
{
	return getChannelIterator(channelName) != channels.end();
}

Channel& ChannelHandler::getChannel(const std::string& channelName) const
{
	channelIter ch = getChannelIterator(channelName);
	if (ch == channels.end())
		throw std::logic_error("must not use ChanndlHandler::find_must_exist(channelName) when not exist");
	return const_cast<Channel&>(*ch);
}

std::set<Channel> ChannelHandler::getChannels() const 
{
	return channels;
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
                channels.insert(Channel(cmd, channelList[i], client, ch_pass[i]));//새로운 채널을 생성해서 넣는다
        }
	}
}

void ChannelHandler::removeTerminatedClient(Client &client)
{
	for(channelIter channelIter = channels.begin(); channelIter != channels.end(); channelIter++)
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


// ?? 아래 두 함수 합칠 예정
// NICK, QUIT에서 호출된다 뭔가 이상함..

std::set<Client> ChannelHandler::get_same_channel_clients(Client&sender)
{
	std::set<Client> clients;
	std::set<Channel> channels = getChannels();
	for(channelIter it = channels.begin(); it != channels.end(); it++)
	{
		const Channel &channel = *it;
		if (channel.isMember(sender))
			clients.insert(channel.getMembers().begin(), channel.getMembers().end());
	}
	return clients;
}

void ChannelHandler::cmdReplyToSameChannel(Client &sender, const Command &cmd)
{
	const std::set<Client> &receivers = get_same_channel_clients(sender);
	for (clientIter it = receivers.begin(); it != receivers.end(); ++it)
	{
		Client receiver = *it;
    	if (receiver == sender)
			continue;
		receiver.appendToSendBuffer(REP_CMD(sender, cmd));
	}
}
