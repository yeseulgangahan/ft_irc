#include "../../include/Channel.hpp"

Channel::Channel(const Command& cmd, const std::string &name, const Client& client, const std::string &pwd)
    :_name(name), \
    _password(pwd), \
    _topic(""), \
    _limitNum(0), \
    _modeInvite(false), \
    _modeTopic(true), \
    _modeLimit(false),
    _modeKey(false)
{
    _members.insert(client);
    _operators.insert(client);
    
    broadcast(client,  REP_CMD(client, cmd));
    reply(client, getTopicReplyString(client));
    names(client);
}

bool Channel::isOperator(const Client& target) const
{
    return _operators.find(target) != _operators.end();
}

std::string Channel::getTopicReplyString(const Client& sender)
{
    if (_topic == "")
        return RPL_NOTOPIC(sender, (*this));
    else
        return RPL_TOPIC(sender, (*this), _topic);
}

bool Channel::requireOperator(Client& sender) const
{
	if (!isOperator(sender))
	{
        reply(sender, ERR_CHANOPRIVSNEEDED((*this)));
		return false;
	}
	return true;
}

bool Channel::requireTargetInChannel(Client& sender, Client &target) const
{
    if (!is_member(target))
	{
        reply(sender, ERR_USERNOTINCHANNEL(sender, target, (*this)));
        return false;
	}
	return true;
}

bool Channel::requireSenderOnChannel(const Client& sender) const
{
    if (!is_member(sender))
	{
        reply(sender, ERR_NOTONCHANNEL(sender, (*this)));
        return false;
	}
    return true;
}

std::string Channel::getName() const
{
    return _name;
}

const std::set<Client>& Channel::get_members() const
{
    return _members;
}

std::string Channel::get_mode() const
{
    std::string mode = "+";
    if (_modeInvite)
        mode += "i";
    if (_modeTopic)
        mode += "t";
    if (_password != "")
        mode += "k";
    if (_modeLimit)
        mode += "l";
    if (mode == "+")
        return "";
    else
        return mode;
}

std::string Channel::getMembershipString(const Client&client) const
{
    if (!is_member(client))
        throw std::runtime_error("getMembershipString must member");
    if (isOperator(client)) return "@";
    else return "";
}


bool Channel::is_member(const Client& target) const
{
    return _members.find(target) != _members.end();
}

void Channel::removeClient(Client& target)
{
    if (!requireSenderOnChannel(target)) return;
    _members.erase(target);
    if (isOperator(target))
        _operators.erase(target);
}


void Channel::addMember(const Command& cmd, Client& sender, const std::string & pass)
{
    if (is_member(sender)) return ;
    if (_modeInvite && (_invited.find(sender) == _invited.end()))
    {
        reply(sender, ERR_INVITEONLYCHAN(sender, (*this)));
        return ;
    }
    if (_modeLimit && (_limitNum < _members.size() + 1))
    {
        reply(sender, ERR_CHANNELISFULL(sender, (*this)));
        return ;
    }
    if (_modeKey && (_password != pass))
    {
        reply(sender, ERR_BADCHANNELKEY(sender, (*this)));
        return ;
    }
    _members.insert(sender);
    broadcast(sender, REPLY(sender.getUserInfo(), cmd._commandName, _name, ""));
    reply(sender,getTopicReplyString(sender));
    names(sender);
}

void Channel::set_topic(const Command& cmd, Client &sender, const std::string &topic)
{
    if (_modeTopic && !requireOperator(sender)) return;
    _topic = topic;
    reply(sender, getTopicReplyString(sender));
    broadcastExceptSender(sender,  REP_CMD(sender, cmd));
}

void Channel::broadcast(const Client& sender, std::string message) const
{
    if (!requireSenderOnChannel(sender))return;
    for (client_it receiver = _members.begin(); receiver != _members.end(); ++receiver)
        reply(*receiver, message);
}

void Channel::broadcastExceptSender(const Client& sender, std::string message) const
{
    if (!requireSenderOnChannel(sender))return;
    for (client_it receiver = _members.begin(); receiver != _members.end(); ++receiver)
    {
        if (*receiver == sender) 
            continue;
        reply(*receiver, message);
    }
}

void Channel::names(const Client &sender) const
{
	//get_name_list: tiger @rabbit dog
	std::string msg = "";
	for (std::set<Client>::iterator cl_it = get_members().begin(); cl_it != get_members().end(); cl_it++)
	{
		if (cl_it != get_members().begin())
			msg += " ";
		msg += getMembershipString(*cl_it);
		msg += cl_it->getNick();
	}

	reply(sender, RPL_NAMREPLY(sender, (getName()), msg));
	reply(sender, RPL_ENDOFNAMES(sender, (getName())));
}

void Channel::show_topic(Client &sender)
{
    if (!requireSenderOnChannel(sender)) return;
    reply(sender, getTopicReplyString(sender));
}

void Channel::invite(const Command &cmd, Client &sender, Client& target)
{
	if (!requireOperator(sender)) return;
	if (is_member(target))
	{
        reply(sender, ERR_USERONCHANNEL(sender, target, (*this)));
		return ;
	}
	_invited.insert(target);
	reply(sender, RPL_INVITING(sender, target, (*this)));
	reply(target, REP_CMD(sender, cmd));
}

void Channel::kick(const Command&cmd, Client &sender, Client& ban_user)
{
	//if (!requireAuthed(sender)) return;
	//if (!requireNickUser(sender)) return;
	if (!requireTargetInChannel(sender, ban_user)) return;
	if (!requireSenderOnChannel(sender)) return;
	if (!requireOperator(sender)) return;
	broadcast(sender,  REP_CMD(sender, cmd));
	removeClient(ban_user);
}

bool Channel::operator<(const Channel& rhs) const
{
    return this->_name < rhs.getName();
}
