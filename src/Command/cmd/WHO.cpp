#include "../../../include/CmdManager.hpp"

static const std::string &channel_name(const Command &cmd)
{
	return cmd._parameters[0];
}

static void send_who_reply(Client &sender, Channel &channel)
{
	std::set<Client> members = channel.getMembers();

	for (client_it it = members.begin(); it != members.end(); it++)
		reply(sender, RPL_WHOREPLY(sender, channel, (*it)));

	reply(sender, RPL_ENDOFWHO(sender, channel));
}

void CmdManager::who(Client &sender, const Command &cmd)
{
	if (!requireAuthed(sender) && !requireNickUser(sender) &&
		!requireEnoughParams(sender, cmd, 1, 2) &&
		!channelManager.requireExistChannel(sender, channel_name(cmd)))
		return;

	send_who_reply(sender, channelManager.getChannel(channel_name(cmd)));
}
