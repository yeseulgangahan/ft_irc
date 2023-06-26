#include "../../../include/CmdManager.hpp"

static bool requireValidNick(Client &client, const std::string & newNick)
{
	if (newNick.size() > 9 || newNick[0] == '#' || newNick[0] == ':')//채널과 구분하기 위해서
	{
		client.appendToSendBuffer(ERR_ERRONEUSNICKNAME(client));
		return false;
	}
	if (client.isNicknameExist(newNick))
	{
		client.appendToSendBuffer(ERR_NICKNAMEINUSE(client, newNick));
		return false;
	}
	return true;
}

void CmdManager::nick(Client &client, const Command& cmd)
{
	if (!requireAuthed(client)) return;
	if (!requireEnoughParams(client, cmd, 1, 2)) return;

	std::string newNick = cmd._parameters[0];
	if (!requireValidNick(client, newNick)) return;

	channelManager.cmdReplyToSameChannel(client, cmd);//내가 속한 채널마다 보내준다
	client.appendToSendBuffer(REP_CMD(client, cmd));//나한테도 보내준다
	client.setNick(newNick);
}
