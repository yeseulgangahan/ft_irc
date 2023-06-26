#include "../../../include/CmdManager.hpp"


//INVITE nick #channel
void CmdManager::invite(Client& sender, const Command& cmd)
{
	if (!requireAuthed(sender) && !requireNickUser(sender) && \
		!requireEnoughParams(sender, cmd, 2, 3))
		return;

	std::string targetUserName = cmd._parameters[0];
	std::string channel_name = cmd._parameters[1];

	if (!clientManager.requireExistNick(sender, targetUserName) && \
		!channelManager.requireExistChannel(sender, channel_name))
		return;
	
	Client& target_user = clientManager.getClient(targetUserName);
	Channel& channel = channelManager.getChannel(channel_name);

	channel.addInvitedList(cmd, sender, target_user);
}
