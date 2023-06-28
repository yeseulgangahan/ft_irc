#include "../../../include/CmdHandler.hpp"

void CmdHandler::sendQuitMessage(Client&sender, const Command &cmd)
{
	channelHandler.replyToAllPeerClients(sender, cmd);
}

void CmdHandler::quit(Client&client, const Command &cmd)
{
	if (!requireAuthed(client)) return;
	if (!requireRegistrationDone(client)) return;
	if (!requireEnoughParams(client, cmd, 0, 1)) return;
	
	sendQuitMessage(client, cmd);
	client.appendToSendBuffer(REP_CMD(client, cmd));//나한테도 보내준다

	throw std::exception();
}
