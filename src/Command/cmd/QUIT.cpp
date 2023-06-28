#include "../../../include/CmdManager.hpp"

void CmdManager::sendQuitMessage(Client&sender, const Command &cmd)
{
	channelManager.replyToAllPeerClients(sender, cmd);
}

void CmdManager::quit(Client&client, const Command &cmd)
{
	if (!requireAuthed(client) && !requireRegistrationDone(client) && \
		!requireEnoughParams(client, cmd, 0, 1))
		return;
	
	sendQuitMessage(client, cmd);
	client.appendToSendBuffer(REP_CMD(client, cmd));//나한테도 보내준다

	throw std::exception();
}
