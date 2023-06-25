#include "../../../include/CmdManager.hpp"


//INVITE nick #channel
void CmdManager::invite(Client& sender, const Command& cmd)
{
	if (!require_authed(sender) && !require_nick_user(sender) && \
		!require_enough_params(sender, cmd, 2, 3))
		return;

	std::string target_user_name = cmd._parameters[0];
	std::string channel_name = cmd._parameters[1];

	if (!clientManager.require_exist_nick(sender, target_user_name) && \
		!channelManager.require_exist_channel(sender, channel_name))
		return;
	
	Client& target_user = clientManager.get_client_by_nick(target_user_name);
	Channel& channel = channelManager.get_channel(channel_name);

	channel.invite(cmd, sender, target_user);
}
