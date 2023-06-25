#include "CmdManager.hpp"


static bool hasOnePasswordParameter(Client &client, const Command& cmd)
{
	if (cmd._parameters.size() != 1)
	{
		reply(client, ERR_NEEDMOREPARAMS(client, cmd._commandName));
		return (false);
	}

	return (true);
}

static bool isNewClient(Client &client)
{
	if (client.isAuthed())
	{
		reply(client, ERR_ALREADYREGISTRED(client));
		return (false);
	}
	return (true);
}

static bool isInputPasswordCorrect(Client &client, const std::string & inputPassword, const std::string &serverPassword)
{
	if (inputPassword != serverPassword)
	{
		reply(client, ERR_PASSWDMISMATCH(client));
		return (false);
	}
	return (true);
}

void CmdManager::pass(Client &client, const Command& cmd)
{
	if (!isNewClient(client)) return;
	if (!hasOnePasswordParameter(client, cmd)) return;
	
	const std::string &password = cmd._parameters[0];
	if (!isInputPasswordCorrect(client, password, _serverPassword)) return;
	
	client.setisAuthed(true);
	reply(client, RPL_NONE("Password Correct!"));
}