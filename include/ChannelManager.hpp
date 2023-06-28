#ifndef CHANNEL_MANAEGR_HPP
#define CHANNEL_MANAEGR_HPP

#include "Channel.hpp"
#include "Command.hpp"

typedef std::set<Channel>::iterator channelIter;


class ChannelManager
{
	std::set<Channel> channels;
	
	//getter
	channelIter getChannelIterator(const std::string& channelName) const;
	bool isChannelExist(const std::string& channelName) const;

	// 합칠 예정
	std::set<Client> get_same_channel_clients(Client& sender);

public:
	//getter
	Channel& getChannel(const std::string& channelName) const;
	std::set<Channel> getChannels() const;

	// client 관련	
	void addClient(const Command& cmd, Client &client, const std::vector<std::string> &channelList, const std::vector<std::string> &ch_pass);
	void removeTerminatedClient(Client &client);
	void removeDepartedClient(const Command& cmd, Client &sender, std::vector<std::string> &channelList);

	// 메시지 보보내내기기
	void broadcastToChannel(const Command& cmd, Client &sender, const std::string &channel_name);

	// COMMAND에서 사용하면서 에러를 버퍼에 쓰도록 하는 함수들
	bool requireValidChannelName(Client &client, const std::string & channel_name);
	bool requireExistChannel(Client &client, const std::string & channel_name);

	// 합칠 예정
	void cmdReplyToSameChannel(Client &sender, const Command &cmd);
	
};
#endif
