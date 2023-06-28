#include "../../../include/CmdHandler.hpp"


//INVITE nick #channel
void CmdHandler::invite(Client& sender, const Command& cmd)
{
	if (!requireRegistrationDone(sender)) return ;
	if (!requireEnoughParams(sender, cmd, 2, 3)) return ;

	std::string targetUserName = cmd._parameters[0];
	std::string channel_name = cmd._parameters[1];

	if (!clientHandler.requireExistNick(sender, targetUserName)) return;
	if (!channelHandler.requireExistChannel(sender, channel_name)) return;
	
	Client& target_user = clientHandler.getClient(targetUserName);
	Channel& channel = channelHandler.getChannel(channel_name);

	channel.addInvitedList(cmd, sender, target_user);
}
