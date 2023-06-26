#include "../../../include/CmdManager.hpp"

//KICK #channel nick
void CmdManager::kick(Client &sender, const Command& cmd)
{
	if (!requireAuthed(sender) && !requireNickUser(sender) && \
		!requireEnoughParams(sender, cmd, 2, 3))
		return;

	std::string channel_name = cmd._parameters[0];
	std::string kick_userName = cmd._parameters[1];

	if (!channelManager.requireExistChannel(sender, channel_name) && \
		!clientManager.requireExistNick(sender, kick_userName))
		return;

	Channel& channel = channelManager.getChannel(channel_name);
	Client &banUser = clientManager.getClientByNick(kick_userName);
	
	channel.kick(cmd, sender, banUser);
}