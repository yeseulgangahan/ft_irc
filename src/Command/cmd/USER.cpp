#include "../../../include/CmdManager.hpp"

static bool require_yet_set_user(Client &client)
{
	if (client.user_setted)
	{
		reply(client, ERR_ALREADYREGISTRED(client));
		return false;
	}
	return true;
}

void CmdManager::user(Client &client, const Command &cmd)
{
	if (!requireAuthed(client)) return;
	if (!requireEnoughParams(client, cmd, 3, 4, true)) return;

	if (!require_yet_set_user(client)) return;
	client.setUserInfo(cmd._parameters[0], cmd._parameters[1], cmd._parameters[2], cmd._trail);
}