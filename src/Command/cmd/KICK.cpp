#include "../../../include/CmdHandler.hpp"

//KICK #channel nick
void CmdHandler::kick(Client &sender, const Command& cmd)
{
	if (!requireAuthed(sender)) return;
	if (!requireNickUser(sender)) return;
	if (!requireEnoughParams(sender, cmd, 2, 3)) return;

	std::string channel_name = cmd._parameters[0];
	std::string kick_userName = cmd._parameters[1];

	if (!channelHandler.requireExistChannel(sender, channel_name)) return;
	if (!clientHandler.requireExistNick(sender, kick_userName)) return;

	Channel& channel = channelHandler.getChannel(channel_name);
	Client &banUser = clientHandler.getClient(kick_userName);
	
	channel.ejectMember(cmd, sender, banUser);
}