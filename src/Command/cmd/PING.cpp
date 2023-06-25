#include "../../../include/CmdManager.hpp"

static void pong(Client &sender, const Command &cmd)
{
	std::string msg = cmd._parameters[0];
	
	reply(sender, PONG_MSG(msg));
}

void CmdManager::ping(Client &sender, const Command& cmd)
{
	if (!require_authed(sender)) return;
	if (!require_nick_user(sender)) return;
	if (!require_enough_params(sender, cmd, 1, 2)) return;

	pong(sender, cmd);
}
