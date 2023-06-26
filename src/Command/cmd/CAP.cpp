#include "../../../include/CmdManager.hpp"
#include "../../../include/Message.hpp"

static void capLS(Client &sender)
{
	reply(sender, CAP_LS);
}

static void capEND(Client &sender)
{
	if (sender.isAuthed() && sender.nickname_setted && sender.user_setted)
	{
		reply(sender, "Authentication Success!");
		sendWelcomeMessages(sender);
		std::cout << YELLOW << "Client " << sender.getNick() << "(fd " << sender.getClientFd() << ")" << " Connected" << RESET << std::endl;
	}
	else
	{
		reply(sender, "Authentication Fail");
		throw std::exception();
	}
}

void CmdManager::cap(Client &sender, const Command &cmd)
{
	if (!requireEnoughParams(sender, cmd, 1, 3))	return;
	
	std::string sub_cmd = cmd._parameters[0];
	if (sub_cmd == "LS")
		capLS(sender);
	else if (sub_cmd == "END")
		capEND(sender);
}