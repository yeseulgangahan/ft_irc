#include "../../../include/CmdHandler.hpp"

void CmdHandler::sendQuitMessage(Client&sender, const Command &cmd)
{
	channelHandler.cmdReplyToSameChannel(sender, cmd);
}

void CmdHandler::quit(Client&client, const Command &cmd)
{
	if (!requireAuthed(client)) return;
	if (!requireNickUser(client)) return;
	if (!requireEnoughParams(client, cmd, 0, 1)) return;
	
	sendQuitMessage(client, cmd);
	throw std::exception();
}
