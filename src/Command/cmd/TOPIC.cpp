#include "../../../include/CmdManager.hpp"

void CmdManager::topic(Client& sender, const Command& cmd)
{
	if (!requireAuthed(sender)) return;
	if (!requireNickUser(sender)) return;
	if (!requireEnoughParams(sender, cmd, 1, 2)) return;

	std::string channel_name = cmd._parameters[0];
	if (!channelManager.requireExistChannel(sender, channel_name)) return;
	
	Channel &channel = channelManager.getChannel(channel_name);
	if (cmd.hasTrailing())
		channel.setTopic(cmd, sender, cmd._trail);
	else 
		channel.showTopic(sender);

}