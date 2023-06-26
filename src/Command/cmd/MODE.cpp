#include "../../../include/CmdManager.hpp"
#include "../../../include/Mode.hpp"

void CmdManager::modeInvite(Channel &channel, Client &sender, const Command &cmd)
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
void CmdManager::modeOperator(Channel &channel, Client &sender, const Command &cmd)
{
	if (!requireEnoughParams(sender, cmd, 3, 6))
		return;
	std::string flag = getFlag(cmd);
	std::string targetNick = cmd._parameters[2];
	if (!clientManager.requireExistNick(sender, targetNick))
		return;
	Client &target = clientManager.getClient(targetNick);
	if (flag == MODE_O_ADD)
		channel.modeOperator(cmd, sender, true, target);
	else if (flag == MODE_O_REM)
		channel.modeOperator(cmd, sender, false, target);
	else
		throw std::logic_error("");
}

void CmdManager::modeTopic(Channel &channel, Client &sender, const Command &cmd)
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

void CmdManager::modeKey(Channel &channel, Client &sender, const Command &cmd)
{
	if (!requireEnoughParams(sender, cmd, 2, 6))
		return;
	std::string flag = getFlag(cmd);
	if (flag == MODE_K)
		channel.modeKey(sender);
	else if (flag == MODE_K_ADD)
	{
		if (!requireEnoughParams(sender, cmd, 3, 6))
			return;
		channel.modeKeyAdd(cmd, sender, cmd._parameters[2]);
	}
	else if (flag == MODE_K_REM)
	{
		channel.modeKeyRemove(cmd, sender);
	}
	else
		throw std::logic_error("");
}

void CmdManager::modeLimit(Channel &channel, Client &sender, const Command &cmd)
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

void CmdManager::modeState(Channel &channel, Client &sender)
{
	sender.appendToSendBuffer(RPL_CHANNELMODEIS(sender, channel, channel.getModeString(), ""));
}

void CmdManager::mode(Client &sender, const Command &cmd)
{
	if (!requireAuthed(sender))
		return;
	if (!requireNickUser(sender))
		return;
	if (!requireEnoughParams(sender, cmd, 1))
		return;
	if (cmd._parameters[0] == "" || cmd._parameters[0][0] != '#' || cmd._parameters[0] == "#")
		return; 
	
	if (!channelManager.requireExistChannel(sender, cmd._parameters[0]))
		return;

	Channel &channel = channelManager.getChannel(cmd._parameters[0]);

	if (cmd._parameters.size() == 1)
		modeState(channel, sender);
	else if (cmd._parameters[1][0] == '+')
		plusOption(channel, sender, cmd);
	else if (cmd._parameters[1][0] == '-')
		minusOption(channel, sender, cmd);
}
// MODE #channel option [target_user]
