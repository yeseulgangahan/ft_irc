#include "../../include/CmdManager.hpp"

CmdManager::CmdManager(ClientManager &clientManager, ChannelManager &channelManager, const std::string &serverPassword)
: clientManager(clientManager), channelManager(channelManager), _serverPassword(serverPassword) { }

std::vector<Command> CmdManager::parse_commands(const std::string &commands_msg)
{
    std::vector<std::string> cmd_lines = splitByLines(commands_msg);
    std::vector<Command> cmds;

    for(size_t i = 0; i < cmd_lines.size(); i++)
        cmds.push_back(Command(cmd_lines[i]));
    return cmds;
}

void CmdManager::executeCommand(Client &sender, const Command &cmd)
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
		default :
					reply(sender, ERR_UNKNOWNCOMMAND(sender, cmd._commandName));
					break;
	}
}

bool CmdManager::require_authed(Client &client)
{
	if (!client.isAuthed())
	{
		reply(client, RPL_NONE("you must authenticate. by PASS <password>"));
		return false;
	}
	return true;
}

bool CmdManager::require_nick_user(Client &client)
{
	if (!client.user_setted || !client.nickname_setted)
	{
		reply(client, ERR_NOTREGISTERED(client));
		return false;
	}
	return true;
}

bool CmdManager::require_enough_params(Client &sender, const Command& cmd, size_t ok_size_min, size_t ng_size_min, bool require_trailing)
{
	assert(ok_size_min < ng_size_min);

	size_t param_size = cmd._parameters.size();

	bool ok = true;
	ok &= (ok_size_min <= param_size) && (param_size < ng_size_min);
	if (require_trailing)
		ok &= cmd.has_trailing();

	if (!ok)
	{
		reply(sender, ERR_NEEDMOREPARAMS(sender, cmd._commandName));
		return false;
	}
	return true;
}


void CmdManager::plus_option(Channel &channel, Client &sender, const Command &cmd)
{

	for (size_t i = 1; i < cmd._parameters[1].size(); i ++)
	{
		switch (cmd._parameters[1][i])
		{
			case 'i':	channel.mode_i(cmd, sender, true);				break;
			case 'o':	
						if (!require_enough_params(sender, cmd, 3, 6))
							return;
						if (!clientManager.require_exist_nick(sender, cmd._parameters[2]))
							return;
						channel.mode_o(cmd, sender, true, \
						clientManager.get_client_by_nick(cmd._parameters[2]));
																		break;
			case 't':
						if (!require_enough_params(sender, cmd, 2, 6))
							return;
						channel.mode_t(cmd, sender, true);				break;

			case 'k':
						if (!require_enough_params(sender, cmd, 2, 6))
							return;
						if (!require_enough_params(sender, cmd, 3, 6))
							return;
						// cmd._parameters.
						channel.mode_k_add(cmd, sender, cmd._parameters[2]);
																		break;
			case 'l':
						if (!require_enough_params(sender, cmd, 2, 6))
							return;
						if (!require_enough_params(sender, cmd, 3, 6))
							return;
						channel.mode_l_add(cmd, sender, cmd._parameters[2]);
																		break;
			default :	break;
		}
	}	
}

void CmdManager::minus_option(Channel &channel, Client &sender, const Command &cmd)
{
	for (size_t i = 1; i < cmd._parameters[1].size(); i ++)
	{
		switch (cmd._parameters[1][i])
		{
			case 'i':	channel.mode_i(cmd, sender, false);				break;
			case 'o':	
						if (!require_enough_params(sender, cmd, 3, 6))
							return;
						if (!clientManager.require_exist_nick(sender, cmd._parameters[2]))
							return;
						channel.mode_o(cmd, sender, false, \
						clientManager.get_client_by_nick(cmd._parameters[2]));
																		break;
			case 't':
						if (!require_enough_params(sender, cmd, 2, 6))
							return;
						channel.mode_t(cmd, sender, false);
																		break;
			case 'k':
						if (!require_enough_params(sender, cmd, 2, 6))
							return;
						channel.mode_k_rem(cmd, sender);
																		break;
			case 'l':
						if (!require_enough_params(sender, cmd, 2, 6))
							return;
						channel.mode_l_rem(cmd, sender);
																		break;
			default :	break;
		}
	}
}

std::vector<std::string> ft_split(const std::string& s, const std::vector<std::string>& delimitersVec)
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

std::vector<std::string> ft_split(const std::string& s, const std::string& t)
{
    return (ft_split(s, std::vector<std::string>(1, t)));
}

std::vector<std::string> splitByLines(const std::string& s)
{
    std::vector<std::string> delimiters;
    
	delimiters.push_back(std::string("\r\n"));
    delimiters.push_back(std::string("\n"));
    
	return (ft_split(s, delimiters));
}
