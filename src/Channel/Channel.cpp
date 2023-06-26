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
    replyNamesCommend(client);
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
    if (!isMember(target))
	{
        reply(sender, ERR_USERNOTINCHANNEL(sender, target, (*this)));
        return false;
	}
	return true;
}

bool Channel::requireSenderOnChannel(const Client& sender) const
{
    if (!isMember(sender))
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

const std::set<Client>& Channel::getMembers() const
{
    return _members;
}

std::string Channel::getModeString() const
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
    if (!isMember(client))
        throw std::runtime_error("getMembershipString must member");
    if (isOperator(client)) return "@";
    else return "";
}


bool Channel::isMember(const Client& target) const
{
    return _members.find(target) != _members.end();
}

void Channel::removeMember(Client& target)
{
    if (!requireSenderOnChannel(target)) return;
    _members.erase(target);
    if (isOperator(target))
        _operators.erase(target);
}


void Channel::addMember(const Command& cmd, Client& sender, const std::string & pass)
{
    if (isMember(sender)) return ;
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
    broadcast(sender, REPLY(sender.getUserString(), cmd._commandName, _name, ""));
    reply(sender,getTopicReplyString(sender));
    replyNamesCommend(sender);
}

void Channel::setTopic(const Command& cmd, Client &sender, const std::string &topic)
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

void Channel::replyNamesCommend(const Client &sender) const
{
	//get_name_list: tiger @rabbit dog
	std::string msg = "";
	for (std::set<Client>::iterator clentIter = getMembers().begin(); clentIter != getMembers().end(); clentIter++)
	{
		if (clentIter != getMembers().begin())
			msg += " ";
		msg += getMembershipString(*clentIter);
		msg += clentIter->getNick();
	}

	reply(sender, RPL_NAMREPLY(sender, (getName()), msg));
	reply(sender, RPL_ENDOFNAMES(sender, (getName())));
}

void Channel::showTopic(Client &sender)
{
    if (!requireSenderOnChannel(sender)) return;
    reply(sender, getTopicReplyString(sender));
}

void Channel::addInvitedList(const Command &cmd, Client &sender, Client& target)
{
	if (!requireOperator(sender)) return;
	if (isMember(target))
	{
        reply(sender, ERR_USERONCHANNEL(sender, target, (*this)));
		return ;
	}
	_invited.insert(target);
	reply(sender, RPL_INVITING(sender, target, (*this)));
	reply(target, REP_CMD(sender, cmd));
}

void Channel::ejectMember(const Command&cmd, Client &sender, Client& banUser)
{
	if (!requireTargetInChannel(sender, banUser)) return;
	if (!requireSenderOnChannel(sender)) return;
	if (!requireOperator(sender)) return;
	broadcast(sender, REP_CMD(sender, cmd));
	removeMember(banUser);
}

bool Channel::operator<(const Channel& rhs) const
{
    return this->_name < rhs.getName();
}
