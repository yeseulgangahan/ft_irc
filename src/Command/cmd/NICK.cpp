#include "../../../include/CmdManager.hpp"

static bool require_valid_nick(Client &client, const std::string & newNick)
{
	if (newNick.size() > 9 || newNick[0] == '#' || newNick[0] == ':')//채널과 구분하기 위해서
	{
		reply(client, ERR_ERRONEUSNICKNAME(client));
		return false;
	}
	if (client.isNicknameExist(newNick))
	{
		reply(client, ERR_NICKNAMEINUSE(client, newNick));
		return false;
	}
	return true;
}

static void setNick(Client& client, const std::string &newNick, const Command& cmd, ChannelManager& channelManager)
{
	channelManager.cmd_reply_to_same_channel(client, cmd);
	reply(client, REP_CMD(client, cmd));
	client.setNick(newNick);
}

void CmdManager::nick(Client &client, const Command& cmd)
{
	if (!requireAuthed(client)) return;
	if (!requireEnoughParams(client, cmd, 1, 2)) return;

	std::string newNick = cmd._parameters[0];
	if (!require_valid_nick(client, newNick)) return;

	setNick(client, newNick, cmd, channelManager);
}
