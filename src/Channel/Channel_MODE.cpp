#include "../../include/Channel.hpp"

void Channel::mode_i_state(Client &client)
{
    if (!requireSenderOnChannel(client))
        return;
    reply(client, RPL_CHANNELMODEIS(client, (*this), (_modeInvited ? "+i" : "-i"), ""));
}

void Channel::mode_i(const Command &cmd, Client &sender, bool valid)
{
    if (!requireOperator(sender))
        return;
    _modeInvited = valid;
    broadcast(sender,  REP_CMD(sender, cmd));
}

void Channel::mode_o(const Command &cmd, Client &sender, bool valid, Client &target)
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

void Channel::mode_t_state(Client &sender)
{
    reply(sender, getTopicReplyString(sender));
}

void Channel::mode_t(const Command &cmd, Client &sender, bool valid)
{
    if (!requireOperator(sender))
        return;
    _modeTopic = valid;
    broadcast(sender,  REP_CMD(sender, cmd));
}

void Channel::mode_k_state(Client &sender)
{
    if (!requireSenderOnChannel(sender))
        return;
    std::string mode;
    if (_password == "")
        mode = "-k";
    else
        mode = "+k";
    reply(sender, RPL_CHANNELMODEIS(sender, (*this), mode, ""));
}

void Channel::mode_k_add(const Command &cmd, Client &sender, const std::string &new_pass)
{
    if (!requireOperator(sender))
        return;
    _password = new_pass;
    broadcast(sender,  REP_CMD(sender, cmd));
}

void Channel::mode_k_rem(const Command &cmd, Client &sender)
{
    if (!requireOperator(sender))
        return;
    _password = "";
    broadcast(sender,  REP_CMD(sender, cmd));
}

void Channel::mode_l_state(Client &sender)
{
    if (!requireSenderOnChannel(sender))
        return;
    if (!_modeLimit)
        return;
    reply(sender, RPL_CHANNELMODEIS(sender, (*this), "+l", std::to_string(_limitNum)));
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

bool Channel::require_valid_num(Client &sender, const std::string &string)
{
    if (!checkDigit(string))
    {
        reply(sender, ERR_NEEDMOREPARAMS(sender, "MODE " + getName() + " +l " + string));
        return false;
    }
    return true;
}

void Channel::mode_l_add(const Command &cmd, Client &sender, const std::string &string)
{
    if (!requireOperator(sender))
        return;
    if (!require_valid_num(sender, string))
        return;
    _modeLimit = true;
    _limitNum = std::atoi(string.c_str());
    broadcast(sender,  REP_CMD(sender, cmd));
}

void Channel::mode_l_rem(const Command &cmd, Client &sender)
{
    if (!requireOperator(sender))
        return;
    _modeLimit = false;
    _limitNum = 0;
    broadcast(sender,  REP_CMD(sender, cmd));
}