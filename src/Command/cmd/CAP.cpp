#include "../../../include/CmdManager.hpp"
#include "../../../include/Message.hpp"

static void capLS(Client &sender)
{
	sender.appendToSendBuffer(CAP_LS);
}

static void capEND(Client &sender)
// 기능협상 지원하지 않는 서버는, NICK, USER를 보내서 등록을 완료한다. 우리 클라이언트는 CAP END를 보내긴 하나 논리적으로 고민됨. 수정할 경우 NICK, USER 둘 중 하나에서 아래 내용 추가.
{
	if (sender.isAuthed() && sender.isNicknameSetted() && sender.isUserSetted())
	{
		sender.setIsRegistrationDone();//등록 종료
		sender.appendToSendBuffer("** Registration Success **");//인증: 패스워드, 등록: 패스 닉 유저
		sender.appendToSendBuffer(RPL_WELCOME(sender));
		sender.appendToSendBuffer(RPL_YOURHOST(sender));
		// sender.appendToSendBuffer(RPL_CREATED(sender));
		// sender.appendToSendBuffer(RPL_MYINFO(sender));
		std::cout << YELLOW << "Client " << sender.getNick() << "(fd " << sender.getFd() << ")" << " Connected" << RESET << std::endl;
	}
	else
	{
		sender.appendToSendBuffer("** Registration Fail **");
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