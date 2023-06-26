#include "../../../include/CmdManager.hpp"

static void pong(Client &sender, const Command &cmd)
{
	std::string msg = cmd._parameters[0];
	
	sender.appendToSendBuffer(PONG_MSG(msg));
}

void CmdManager::ping(Client &sender, const Command& cmd)
{
	if (!requireAuthed(sender)) return;
	if (!requireNickUser(sender)) return;
	if (!requireEnoughParams(sender, cmd, 1, 2)) return;

	pong(sender, cmd);
}
