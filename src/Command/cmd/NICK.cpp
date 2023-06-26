#include "../../../include/CmdManager.hpp"

static bool require_valid_nick(Client &client, const std::string & new_nick)
{
	if (new_nick.size() > 9)
	{
		reply(client, ERR_ERRONEUSNICKNAME(client));
		return false;
	}
	if (client.IsNicknameExist(new_nick))
	{
		reply(client, ERR_NICKNAMEINUSE(client, new_nick));
		return false;
	}
	return true;
}

static void setNick(Client& client, const std::string &new_nick, const Command& cmd, ChannelManager& channelManager)
{
	channelManager.cmd_reply_to_same_channel(client, cmd);
	reply(client, REP_CMD(client, cmd));
	client.setNick(new_nick);
}

void CmdManager::nick(Client &client, const Command& cmd)
{
	if (!requireAuthed(client)) return;
	if (!requireEnoughParams(client, cmd, 1, 2)) return;

	std::string new_nick = cmd._parameters[0];
	if (!require_valid_nick(client, new_nick)) return;

	setNick(client, new_nick, cmd, channelManager);
}
