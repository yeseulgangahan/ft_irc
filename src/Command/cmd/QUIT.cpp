#include "../../../include/CmdManager.hpp"

void CmdManager::sendQuitMessage(Client&sender, const Command &cmd)
{
	channelManager.cmdReplyToSameChannel(sender, cmd);
}

void CmdManager::quit(Client&client, const Command &cmd)
{
	if (!requireAuthed(client)) return;
	if (!requireNickUser(client)) return;
	if (!requireEnoughParams(client, cmd, 0, 1)) return;
	
	sendQuitMessage(client, cmd);
	throw std::exception();
}
