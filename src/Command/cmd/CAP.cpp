#include "../../../include/CmdManager.hpp"
#include "../../../include/Message.hpp"

static void capLS(Client &sender)
{
	reply(sender, CAP_LS);
}

static void capEND(Client &sender)
{
	if (sender.isAuthed() && sender.isNicknameSetted() && sender.isUserSetted())
	{
		sendWelcomeMessages(sender);
		sender.setIsRegistrationDone();//메시지 다 보내고 등록 종료
		reply(sender, "Registration Success!");//인증: 패스워드, 등록: 패스 닉 유저
		std::cout << YELLOW << "Client " << sender.getNick() << "(fd " << sender.getFd() << ")" << " Connected" << RESET << std::endl;
	}
	else
	{
		reply(sender, "Registration Fail");
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