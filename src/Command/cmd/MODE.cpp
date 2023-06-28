#include "../../../include/CmdHandler.hpp"
#include "../../../include/Mode.hpp"

void CmdHandler::modeInvite(Channel &channel, Client &sender, const Command &cmd)
{
	std::string flag = getFlag(cmd);
	if (flag == MODE_I)
		channel.modeInviteState(sender);
	else if (flag == MODE_I_ADD)
		channel.modeInvite(cmd, sender, true);
	else if (flag == MODE_I_REM)
		channel.modeInvite(cmd, sender, false);
	else
		throw std::logic_error("");
}
//MODE #ch1 +o user
void CmdHandler::modeOperator(Channel &channel, Client &sender, const Command &cmd)
{
	if (!requireEnoughParams(sender, cmd, 3, 6))
		return;
	std::string flag = getFlag(cmd);
	std::string targetNick = cmd._parameters[2];
	if (!clientHandler.requireExistNick(sender, targetNick))
		return;
	Client &target = clientHandler.getClient(targetNick);
	if (flag == MODE_O_ADD)
		channel.modeOperator(cmd, sender, true, target);
	else if (flag == MODE_O_REM)
		channel.modeOperator(cmd, sender, false, target);
	else
		throw std::logic_error("");
}

void CmdHandler::modeTopic(Channel &channel, Client &sender, const Command &cmd)
{
	if (!requireEnoughParams(sender, cmd, 2, 6))
		return;
	std::string flag = getFlag(cmd);
	if (flag == MODE_T)
		channel.modeTopicState(sender);
	else if (flag == MODE_T_ADD)
		channel.modeTopic(cmd, sender, true);
	else if (flag == MODE_T_REM)
		channel.modeTopic(cmd, sender, false);
	else
		throw std::logic_error("");
}


void CmdHandler::modeLimit(Channel &channel, Client &sender, const Command &cmd)
{
	if (!requireEnoughParams(sender, cmd, 2, 6))
		return;
	std::string flag = getFlag(cmd);
	if (flag == MODE_L)
		channel.modeLimitState(sender);
	else if (flag == MODE_L_ADD)
	{
		if (!requireEnoughParams(sender, cmd, 3, 6))
			return;
		channel.modeLimitAdd(cmd, sender, cmd._parameters[2]);
	}
	else if (flag == MODE_L_REM)
	{
		channel.modeLimitRemove(cmd, sender);
	}
	else
		throw std::logic_error("");
}

void CmdHandler::modeState(Channel &channel, Client &sender)
{
	sender.appendToSendBuffer(RPL_CHANNELMODEIS(sender, channel, channel.getModeString(), ""));
}

void CmdHandler::mode(Client &sender, const Command &cmd)
{
	if (!requireAuthed(sender))
		return;
	if (!requireRegistrationDone(sender))
		return;
	if (!requireEnoughParams(sender, cmd, 1))
		return;

	// user관련 모드는 처리 안 함
	if (clientHandler.isClientExistByNick(cmd._parameters[0]))
		return ;
	
	// valid한 채널인지 확인할 필요 없음. 있는 user가 아니면 채널이라고 가정하고, channel도 아니면 없는 채널이라고 답장해줌 (상용과 비슷)
	if (!channelHandler.requireExistChannel(sender, cmd._parameters[0]))
		return;

	Channel &channel = channelHandler.getChannel(cmd._parameters[0]);

	if (cmd._parameters.size() == 1)
		modeState(channel, sender);
	else if (cmd._parameters[1][0] == '+')
		plusOption(channel, sender, cmd);
	else if (cmd._parameters[1][0] == '-')
		minusOption(channel, sender, cmd);
}
// MODE #channel option [target_user]
