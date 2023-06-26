#include "../../../include/CmdManager.hpp"

void CmdManager::sendQuitMessage(Client&sender, const Command &cmd)
{
	channelManager.cmd_reply_to_same_channel(sender, cmd);
}

void CmdManager::quit(Client&client, const Command &cmd)
{
	if (!requireAuthed(client) && !requireNickUser(client) && \
		!requireEnoughParams(client, cmd, 0, 1))
		return;
	
	sendQuitMessage(client, cmd);
	throw std::exception();
}
