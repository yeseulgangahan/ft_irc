#include "../../../include/CmdHandler.hpp"

void CmdHandler::topic(Client& sender, const Command& cmd)
{
	if (!requireRegistrationDone(sender)) return;
	if (!requireEnoughParams(sender, cmd, 1, 2)) return;


	std::string channel_name = cmd._parameters[0];
	if (!channelHandler.requireExistChannel(sender, channel_name)) return;
	
	Channel &channel = channelHandler.getChannel(channel_name);
	if (cmd.hasTrailing())
		channel.setTopic(cmd, sender, cmd._trail);
	else 
		channel.showTopic(sender);

}