#include "Channel.hpp"
#include "ChannelManager.hpp"

//채널에 sender 제외하고 커맨드를 브로드캐스트하기
void ChannelManager::notice_to_channel(const Command &cmd, Client &sender, const std::string &channel_name)
{
	if (!require_exist_channel(sender, channel_name))
		return;
	get_channel(channel_name).broadcastExceptSender(sender,  REP_CMD(sender, cmd));
}