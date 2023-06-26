#include "../../include/Channel.hpp"

Channel::Channel(const Command& cmd, const std::string &name, Client& client, const std::string &pwd)
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
    client.appendToSendBuffer(getTopicReplyString(client));
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
        sender.appendToSendBuffer(ERR_CHANOPRIVSNEEDED((*this)));
		return false;
	}
	return true;
}

bool Channel::requireTargetInChannel(Client& sender, Client &target) const
{
    if (!isMember(target))
	{
        sender.appendToSendBuffer(ERR_USERNOTINCHANNEL(sender, target, (*this)));
        return false;
	}
	return true;
}

bool Channel::requireSenderOnChannel(Client& sender) const
{
    if (!isMember(sender))
	{
        sender.appendToSendBuffer(ERR_NOTONCHANNEL(sender, (*this)));
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

std::string Channel::getModeString()
{
    std::string mode = "+";
    std::stringstream ss;
    bool k = false;
    bool l = false;

    if (_modeInvite)
        mode += "i";
    if (_modeTopic)
        mode += "t";
    if (_password != "")
    {
        k = true;
        mode += "k";
    }
    if (_modeLimit)
    {
        l = true;
        mode += "l";
    }
    if (mode == "+")
        return "";
    else
    {
        if (k)
        {
            ss << _password;
            mode += " " + ss.str();
            ss.clear();
            ss.str("");
        }
        if (l)
        {
            ss << _limitNum;
            mode += " " + ss.str();
            ss.clear();
            ss.str("");
        }
        return mode;
    }
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
        sender.appendToSendBuffer(ERR_INVITEONLYCHAN(sender, (*this)));
        return ;
    }
    if (_modeLimit && (_limitNum < _members.size() + 1))
    {
        sender.appendToSendBuffer(ERR_CHANNELISFULL(sender, (*this)));
        return ;
    }
    if (_modeKey && (_password != pass))
    {
        sender.appendToSendBuffer(ERR_BADCHANNELKEY(sender, (*this)));
        return ;
    }
    _members.insert(sender);
    broadcast(sender, REPLY(sender.getUserString(), cmd._commandName, _name, ""));
    sender.appendToSendBuffer(getTopicReplyString(sender));
    replyNamesCommend(sender);
}

void Channel::setTopic(const Command& cmd, Client &sender, const std::string &topic)
{
    if (_modeTopic && !requireOperator(sender)) return;
    _topic = topic;
    sender.appendToSendBuffer(getTopicReplyString(sender));
    broadcastExceptSender(sender,  REP_CMD(sender, cmd));
}

void Channel::broadcast(Client& sender, const std::string &message)
{(void)message;
    if (!requireSenderOnChannel(sender))return;
    // for (clientIter receiver = _members.begin(); receiver != _members.end(); ++receiver)
        // (*receiver).appendToRecvBuffer(message);
}

void Channel::broadcastExceptSender(Client& sender, const std::string &message)
{(void)message;
    if (!requireSenderOnChannel(sender)) return;
    for (clientIter receiver = _members.begin(); receiver != _members.end(); ++receiver)
    {
        if (*receiver == sender) 
            continue;
        // (*receiver).appendToRecvBuffer(message);
    }
}

void Channel::replyNamesCommend(Client &sender) const
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

	sender.appendToSendBuffer(RPL_NAMREPLY(sender, (getName()), msg));
	sender.appendToSendBuffer(RPL_ENDOFNAMES(sender, (getName())));
}

void Channel::showTopic(Client &sender)
{
    if (!requireSenderOnChannel(sender)) return;
    sender.appendToSendBuffer(getTopicReplyString(sender));
}

void Channel::addInvitedList(const Command &cmd, Client &sender, Client& target)
{
	if (!requireOperator(sender)) return;
	if (isMember(target))
	{
        sender.appendToSendBuffer(ERR_USERONCHANNEL(sender, target, (*this)));
		return ;
	}
	_invited.insert(target);
	sender.appendToSendBuffer(RPL_INVITING(sender, target, (*this)));
	target.appendToSendBuffer(REP_CMD(sender, cmd));
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
