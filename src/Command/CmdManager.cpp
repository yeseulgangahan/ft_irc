#include "../../include/CmdHandler.hpp"

CmdHandler::CmdHandler(ClientHandler &clientHandler, ChannelHandler &channelHandler, const std::string &serverPassword)
: clientHandler(clientHandler), channelHandler(channelHandler), _serverPassword(serverPassword) { }

std::vector<Command> CmdHandler::parseCommands(const std::string &commands_msg)
{
    std::vector<std::string> cmd_lines = splitByLines(commands_msg);
    std::vector<Command> cmds;

    for(size_t i = 0; i < cmd_lines.size(); i++)
        cmds.push_back(Command(cmd_lines[i]));
    return cmds;
}

void CmdHandler::executeCommand(Client &sender, const Command &cmd)
{
	//cmd.debug();
	switch (cmd._commandName[0])
	{
		case 'C':
					if (cmd._commandName == CAP)
					{	cap(sender, cmd);	break;	}

		case 'P':
					if (cmd._commandName == PRIVMSG)
					{	privmsg(sender, cmd);	break;	}
					else if (cmd._commandName == PING)
					{	ping(sender, cmd);	break;	}
					else if (cmd._commandName == PART)
					{	part(sender, cmd);	break;	}
					else if (cmd._commandName == PASS)		
					{	pass(sender, cmd);	break;	}

		case 'N':
					if (cmd._commandName == NOTICE)
					{	notice(sender, cmd);	break;	}
					else if (cmd._commandName == NICK)		
					{	nick(sender, cmd);		break;	}

		case 'U':
					if (cmd._commandName == USER)		
					{	user(sender, cmd);	break;	}

		case 'J':	
					if (cmd._commandName == JOIN)		
					{	join(sender, cmd);	break;	}

		case 'T':
					if (cmd._commandName == TOPIC)		
					{	topic(sender, cmd);	break;	}

		case 'M':
					if (cmd._commandName == MODE)
					{	mode(sender, cmd);	break;	}

		case 'W':
					if (cmd._commandName == WHO)		
					{	who(sender, cmd);	break;	}

		case 'Q':			
					if (cmd._commandName == QUIT)		
					{	quit(sender, cmd);	break;	}

		case 'K':
					if (cmd._commandName == KICK)		
					{	kick(sender, cmd);	break;	}

		case 'I':
					if (cmd._commandName == INVITE)		
					{	invite(sender, cmd);	break;	}
		//인증 전에는 커맨드를 아예 받지 않아야 한다. 우리 클라이언트는 안 보내준다. nc를 위해 안 됐어도 알려준다.
		default :
					if (sender.isRegistrationDone())//인증완료된 아이는 아래의 에러메시지
					{   sender.appendToSendBuffer(ERR_UNKNOWNCOMMAND(sender, cmd._commandName));	break;	}
					else//인증 아직 안 됐으면
					{   sender.appendToSendBuffer(ERR_NOTREGISTERED(sender));	break;   }
	}
}

//삭제
bool CmdHandler::requireAuthed(Client &client)
{
	if (!client.isAuthed())
	{
		client.appendToSendBuffer(RPL_NONE("Authentication Failed. Try /connect"));
		return false;
	}
	return true;
}

// requireRegistrationDone으로 변경
bool CmdHandler::requireNickUser(Client &client)
{
	if (!client.isRegistrationDone())
	{
		client.appendToSendBuffer(ERR_NOTREGISTERED(client));
		return false;
	}
	return true;
}

bool CmdHandler::requireEnoughParams(Client &sender, const Command& cmd, size_t ok_size_min, size_t ng_size_min, bool require_trailing)
{
	assert(ok_size_min < ng_size_min);

	size_t param_size = cmd._parameters.size();

	bool ok = true;
	ok &= (ok_size_min <= param_size) && (param_size < ng_size_min);
	if (require_trailing)
		ok &= cmd.hasTrailing();

	if (!ok)
	{
		sender.appendToSendBuffer(ERR_NEEDMOREPARAMS(sender, cmd._commandName));
		return false;
	}
	return true;
}


void CmdHandler::plusOption(Channel &channel, Client &sender, const Command &cmd)
{
	int no_arg = 0;
	for (size_t i = 1; i < cmd._parameters[1].size(); i ++)
	{
		switch (cmd._parameters[1][i])
		{
			case 'i':
						no_arg++;
						channel.modeInvite(cmd, sender, true);				break;
			case 'o':	
						if (!requireEnoughParams(sender, cmd, 3, 6))
							return;

						if (no_arg && !clientHandler.requireExistNick(sender, \
							cmd._parameters[1 + i - no_arg]))
							return;
						else if (!clientHandler.requireExistNick(sender, \
								cmd._parameters[1 + i]))
							return;
							
						if (no_arg)
							channel.modeOperator(cmd, sender, true, \
							clientHandler.getClient(cmd._parameters[1 + i - no_arg]));
						else
							channel.modeOperator(cmd, sender, true, \
							clientHandler.getClient(cmd._parameters[1 + i]));
																		break;
			case 't':
						no_arg++;
						if (!requireEnoughParams(sender, cmd, 2, 6))
							return;
						channel.modeTopic(cmd, sender, true);				break;

			case 'k':
						if (!requireEnoughParams(sender, cmd, 2, 6) && \
							!requireEnoughParams(sender, cmd, 3, 6))
							return;

						if (no_arg)
							channel.modeKeyAdd(cmd, sender, cmd._parameters[1 + i - no_arg]);
						else
							channel.modeKeyAdd(cmd, sender, cmd._parameters[1 + i]);
																		break;
			case 'l':
						if (!requireEnoughParams(sender, cmd, 2, 6) && \
							!requireEnoughParams(sender, cmd, 3, 6))
							return;

						if (no_arg)
							channel.modeLimitAdd(cmd, sender, cmd._parameters[1 + i - no_arg]);
						else
							channel.modeLimitAdd(cmd, sender, cmd._parameters[1 + i]);

																		break;
			default :	no_arg++;
						break;
		}
	}	
}

void CmdHandler::minusOption(Channel &channel, Client &sender, const Command &cmd)
{
	for (size_t i = 1; i < cmd._parameters[1].size(); i ++)
	{
		switch (cmd._parameters[1][i])
		{
			case 'i':	channel.modeInvite(cmd, sender, false);				break;
			case 'o':	
						if (!requireEnoughParams(sender, cmd, 3, 6))
							return;
						if (!clientHandler.requireExistNick(sender, cmd._parameters[2]))
							return;
						channel.modeOperator(cmd, sender, false, \
						clientHandler.getClient(cmd._parameters[2]));
																		break;
			case 't':
						if (!requireEnoughParams(sender, cmd, 2, 6))
							return;
						channel.modeTopic(cmd, sender, false);
																		break;
			case 'k':
						if (!requireEnoughParams(sender, cmd, 2, 6))
							return;
						channel.modeKeyRemove(cmd, sender);
																		break;
			case 'l':
						if (!requireEnoughParams(sender, cmd, 2, 6))
							return;
						channel.modeLimitRemove(cmd, sender);
																		break;
			default :	break;
		}
	}
}

std::vector<std::string> ftSplit(const std::string& s, const std::vector<std::string>& delimitersVec)
{
    std::vector<std::string> splited;
    size_t left = 0;

	while(left < s.length())
    {
        size_t right = s.length();
        size_t delimiterLen = 0;
       
		for(size_t j(0); j < delimitersVec.size(); ++j)
        {
            size_t pos = s.find(delimitersVec[j], left);
            if (pos != std::string::npos && pos < right)
            {
                right = pos;
                delimiterLen = delimitersVec[j].length();
            }
        }

        if (left < right)
            splited.push_back(s.substr(left, right - left));
        
		left = right + delimiterLen;
    }
    return (splited);
}

std::vector<std::string> ftSplit(const std::string& s, const std::string& t)
{
    return (ftSplit(s, std::vector<std::string>(1, t)));
}

std::vector<std::string> splitByLines(const std::string& s)
{
    std::vector<std::string> delimiters;
    
	delimiters.push_back(std::string("\r\n"));
    delimiters.push_back(std::string("\n"));
    
	return (ftSplit(s, delimiters));
}
