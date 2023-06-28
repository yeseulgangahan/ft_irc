#include "../../../include/CmdHandler.hpp"

static const std::string &channel_name(const Command &cmd)
{
	return cmd._parameters[0];
}

static void send_who_reply(Client &sender, Channel &channel)
{
	const std::set<Client>& members = channel.getMembers();

	for (clientIter it = members.begin(); it != members.end(); it++)
		sender.appendToSendBuffer(RPL_WHOREPLY(sender, channel, (*it)));

	sender.appendToSendBuffer(RPL_ENDOFWHO(sender, channel));
}

void CmdHandler::who(Client &sender, const Command &cmd)
{
	if (!requireRegistrationDone(sender)) return;
	if (!requireEnoughParams(sender, cmd, 1, 2)) return;
	if (!channelHandler.requireExistChannel(sender, channel_name(cmd))) return;

	send_who_reply(sender, channelHandler.getChannel(channel_name(cmd)));
}
