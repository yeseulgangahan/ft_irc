#include "../../../include/CmdManager.hpp"


static bool hasOnePasswordParameter(Client &client, const Command& cmd)
{
	if (cmd._parameters.size() != 1)
	{
		client.appendToSendBuffer(ERR_NEEDMOREPARAMS(client, cmd._commandName));
		return (false);
	}

	return (true);
}

static bool isNewClient(Client &client)
{
	if (client.isAuthed())
	{
		client.appendToSendBuffer(ERR_ALREADYREGISTRED(client));
		return (false);
	}
	return (true);
}

static bool isInputPasswordCorrect(Client &client, const std::string & inputPassword, const std::string &serverPassword)
{
	if (inputPassword != serverPassword)
	{
		client.appendToSendBuffer(ERR_PASSWDMISMATCH(client));
		return (false);
	}
	return (true);
}

void CmdManager::pass(Client &client, const Command& cmd)
{
	if (!isNewClient(client)) return;//이미 인증됐으면 안됨
	if (!hasOnePasswordParameter(client, cmd)) return;
	
	const std::string &password = cmd._parameters[0];
	if (!isInputPasswordCorrect(client, password, _serverPassword)) return;
	
	client.setIsAuthed();
	client.appendToSendBuffer(RPL_NONE("Password Correct!"));
}