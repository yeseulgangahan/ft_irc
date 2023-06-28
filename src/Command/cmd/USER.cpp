#include "../../../include/CmdManager.hpp"

void CmdManager::user(Client &client, const Command &cmd)
{
	if (!requireAuthed(client)) return;
	if (client.isUserSetted())
	{
		client.appendToSendBuffer(ERR_ALREADYREGISTRED(client));
		return ;
	}
	if (!requireEnoughParams(client, cmd, 3, 4, true)) return;
	client.setUserInfo(cmd._parameters[0], cmd._parameters[1], cmd._parameters[2], cmd._trail);
}