#include "ChannelManager.hpp"

channel_it ChannelManager::getChannelIterator(std::string channelName) const
{
	for(channel_it it = channels.begin(); it != channels.end(); it++)
		if (it->getName() == channelName)
			return it;
	return channels.end();
}

bool ChannelManager::isChannelExist(std::string channelName) const
{
	return getChannelIterator(channelName) != channels.end();
}

Channel& ChannelManager::get_channel(std::string channelName) const
{
	channel_it ch = getChannelIterator(channelName);
	if (ch == channels.end())
		throw std::logic_error("must not use ChanndlManager::find_must_exist(channelName) when not exist");
	return const_cast<Channel&>(*ch);
}

std::set<Channel> ChannelManager::get_all_channels() const 
{
	return channels;
}

//join에 해당하는 채널매니저의 동작: add participant
void ChannelManager::join(const Command& cmd, Client &client, const std::vector<std::string> &channelList, const std::vector<std::string> &ch_pass)
{
	for(size_t i = 0; i < channelList.size(); i++)
	{
		if (require_valid_channel_name(client, channelList[i]))//채널이름 규칙에 맞는지
		{
            if (isChannelExist(channelList[i]))//존재하는 채널이면
                get_channel(channelList[i]).addMember(cmd, client, ch_pass[i]);//그 채널에 유저 추가
            else
                channels.insert(Channel(cmd, channelList[i], client, ch_pass[i]));//새로운 채널을 생성해서 넣는다
        }
	}
}

void ChannelManager::remove_by_all_channel(Client &client)
{
	for(channel_it channel_it = channels.begin(); channel_it != channels.end(); channel_it++)
	{
		Channel &channel = const_cast<Channel&>(*channel_it);

		if (channel.is_member(client))//체널의 멤버면 멤버를 지운다
		{
			channel.removeClient(client);
			if (channel.get_members().empty())//채널이 텅 비었으면 채널도 지운다
				get_all_channels().erase(channel);
		}
	}
}

void ChannelManager::part_list(const Command& cmd, Client &sender, std::vector<std::string> &channelList)
{
	for(size_t i = 0; i < channelList.size(); i++)
	{
		if (require_exist_channel(sender, channelList[i]))//존재하는 채널이면
		{
			get_channel(channelList[i]).broadcast(sender, REPLY(sender.get_user_info(), cmd._commandName, channelList[i], ""));//해당 채널에서 PART 메시지를 보내야 한다
			get_channel(channelList[i]).removeClient(sender);//떠난다

		}
	}
}

bool ChannelManager::require_valid_channel_name(Client &client, const std::string & channel_name)
{
	if ((channel_name == "" || channel_name[0] != '#' || channel_name == "#"))
	{
		reply(client, ERR_NOSUCHCHANNEL(client, channel_name));
		return false;
	}
	return true;
}

bool ChannelManager::require_exist_channel(Client &client, const std::string & channel_name)
{
	if (!isChannelExist(channel_name))
	{
		reply(client, ERR_NOSUCHCHANNEL(client, channel_name));
		return false;
	}
	return true;
}

void ChannelManager::broadcastToChannel(const Command &cmd, Client &sender, const std::string &channel_name)
{
	if (!require_exist_channel(sender, channel_name))
		return;
	get_channel(channel_name).broadcastExceptSender(sender,  REP_CMD(sender, cmd));
}


// ?? 아래 두 함수 합칠 예정
// NICK, QUIT에서 호출된다 뭔가 이상함..

std::set<Client> ChannelManager::get_same_channel_clients(Client&sender)
{
	std::set<Client> clients;
	std::set<Channel> channels = get_all_channels();
	for(channel_it it = channels.begin(); it != channels.end(); it++)
	{
		const Channel &channel = *it;
		if (channel.is_member(sender))
			clients.insert(channel.get_members().begin(), channel.get_members().end());
	}
	return clients;
}

void ChannelManager::cmd_reply_to_same_channel(Client&sender, const Command &cmd)
{
	const std::set<Client> &receivers = get_same_channel_clients(sender);
	for (client_it it = receivers.begin(); it != receivers.end(); ++it)
	{
		Client receiver = *it;
    	if (receiver == sender)
			continue;
		reply(receiver, REP_CMD(sender, cmd));
	}
}
