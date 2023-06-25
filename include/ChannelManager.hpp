#ifndef CHANNEL_MANAEGR_HPP
#define CHANNEL_MANAEGR_HPP

#include "Channel.hpp"
#include "Command.hpp"

typedef std::set<Channel>::iterator channel_it;


class ChannelManager
{
	std::set<Channel> channels;
	
	//getter
	channel_it getChannelIterator(std::string channelName) const;
	bool isChannelExist(std::string channelName) const;

	//*** 삭제: name 관련 함수
	void names_all_channel(const Client &sender, std::set<Client> &all_clients);
	
	// 합칠 예정
	std::set<Client> get_same_channel_clients(Client&sender);

public:
	//getter
	Channel& get_channel(std::string channelName) const;//*** getChannel() 로 변경
	std::set<Channel> get_all_channels() const;//*** getChannels()로 변경
	
	void join(const Command& cmd, Client &client, const std::vector<std::string> &channelList, const std::vector<std::string> &ch_pass);	//*** addClient()로 이름 변경
	void remove_by_all_channel(Client &client);//*** removeTerminatedClient()
	void part_list(const Command& cmd, Client &sender, std::vector<std::string> &channelList);//*** removeDepartedClient()

	// COMMAND에서 사용하면서 에러를 버퍼에 쓰도록 하는 함수들
	bool require_valid_channel_name(Client &client, const std::string & channel_name);
	bool require_exist_channel(Client &client, const std::string & channel_name);

	// 합칠 예정
	void cmd_reply_to_same_channel(Client &sender, const Command &cmd);
	
	//*** 추가
	// void broadcastToChannel(const Command& cmd, Client &sender, const std::string &channel_name); //채널에 전체 메시지 보내기
	
	// *** 삭제
	void names_channel(Client &client, const Command& cmd);
 	void names_all(const Client &sender, std::set<Client> all_clients);
	void privmsg_to_channel(const Command& cmd, Client &sender, const std::string &channel_name);//broadcastToChannel로 전체 변경
	void notice_to_channel(const Command& cmd, Client &sender, const std::string &channel_name);//broadcastToChannel로 전체 변경
	bool is_valid_channel_name(const std::string & channel_name);//command에서 사용하는 거 변경하면 삭제가능
};
#endif
