#include "../../include/Channel.hpp"

void Channel::modeInviteState(Client &client)
{
    if (!requireSenderOnChannel(client))
        return;
    client.appendToSendBuffer(RPL_CHANNELMODEIS(client, (*this), (_modeInvite ? "+i" : "-i"), ""));
}

void Channel::modeInvite(const Command &cmd, Client &sender, bool valid)
{
    if (!requireOperator(sender))
        return;
    _modeInvite = valid;
    broadcast(sender,  REP_CMD(sender, cmd));
}

void Channel::modeOperator(const Command &cmd, Client &sender, bool valid, Client &target)
{
    if (!requireOperator(sender))
        return;
    if (!requireSenderOnChannel(sender))
        return;
    if (!requireTargetInChannel(sender, target))
        return;
    broadcast(sender,  REP_CMD(sender, cmd));
    if (valid)
    {
        _operators.insert(target);
    }
    else
    {
        if (_operators.count(target))
            _operators.erase(target);
    }
}

void Channel::modeTopicState(Client &sender)
{
    sender.appendToSendBuffer(getTopicReplyString(sender));
}

void Channel::modeTopic(const Command &cmd, Client &sender, bool valid)
{
    if (!requireOperator(sender))
        return;
    _modeTopic = valid;
    broadcast(sender,  REP_CMD(sender, cmd));
}

void Channel::modeKey(Client &sender)
{
    if (!requireSenderOnChannel(sender))
        return;
    std::string mode;
    if (_password == "")
        mode = "-k";
    else
        mode = "+k";
    sender.appendToSendBuffer(RPL_CHANNELMODEIS(sender, (*this), mode, ""));
}

void Channel::modeKeyAdd(const Command &cmd, Client &sender, const std::string &new_pass)
{
    if (!requireOperator(sender))
        return;
    _password = new_pass;
    _modeKey = true;
    broadcast(sender,  REP_CMD(sender, cmd));
}

void Channel::modeKeyRemove(const Command &cmd, Client &sender)
{
    if (!requireOperator(sender))
        return;
    _password = "";
    _modeKey = false;
    broadcast(sender,  REP_CMD(sender, cmd));
}

void Channel::modeLimitState(Client &sender)
{
    if (!requireSenderOnChannel(sender))
        return;
    if (!_modeLimit)
        return;
    sender.appendToSendBuffer(RPL_CHANNELMODEIS(sender, (*this), "+l", std::to_string(_limitNum)));
}

static bool checkDigit(std::string string)
{
	int len = string.length();
	for (int i(0); i < len; ++i)
		if (!isdigit(string[i]))
			return (false);
		
	int Num = std::atoi(string.c_str());
	if (Num < 0 || Num > 1000)
		return (false);
	
	return (true);
}

void Channel::modeLimitAdd(const Command& cmd, Client& sender, const std::string& string)
{
    if (!requireOperator(sender))
        return;
    if (!checkDigit(string))
    {
        sender.appendToSendBuffer(ERR_NEEDMOREPARAMS(sender, "MODE " + getName() + " +l " + string));
        return ;
    }
    _modeLimit = true;
    _limitNum = std::atoi(string.c_str());
    broadcast(sender, REP_CMD(sender, cmd));
}

void Channel::modeLimitRemove(const Command& cmd, Client& sender)
{
    if (!requireOperator(sender))
        return;
    _modeLimit = false;
    _limitNum = 0;
    broadcast(sender, REP_CMD(sender, cmd));
}