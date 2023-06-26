#ifndef MODE_HPP
#define MODE_HPP
#include <iostream>
#include <algorithm>
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <string> 
#include <fcntl.h>
#include <signal.h>
#include <sys/poll.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "Command.hpp"

const std::string MODE_I_ADD = "+i";
const std::string MODE_I_REM = "-i";
const std::string MODE_I = "i";
const std::string MODE_O_ADD = "+o";
const std::string MODE_O_REM = "-o";
const std::string MODE_T = "t";
const std::string MODE_T_ADD = "+t";
const std::string MODE_T_REM = "-t";
const std::string MODE_K = "k";
const std::string MODE_K_ADD = "+k";
const std::string MODE_K_REM = "-k";
const std::string MODE_L = "l";
const std::string MODE_L_ADD = "+l";
const std::string MODE_L_REM = "-l";

std::string getFlag(const Command &cmd)
{
	assert(cmd._parameters.size() > 1);
	return cmd._parameters[1];
}

bool isModeInvite(const Command &cmd)
{
	std::string flag = getFlag(cmd);
	return flag == MODE_I_ADD || flag == MODE_I_REM || flag == MODE_I;
}

bool isModeOperator(const Command &cmd)
{
	std::string flag = getFlag(cmd);
	return flag == MODE_O_ADD || flag == MODE_O_REM;
}

bool isModeTopic(const Command &cmd)
{
	std::string flag = getFlag(cmd);
	return flag == MODE_T_ADD || flag == MODE_T_REM || flag == MODE_T;
}

bool isModeKey(const Command &cmd)
{
	std::string flag = getFlag(cmd);
	return flag == MODE_K_ADD || flag == MODE_K_REM || flag == MODE_K;
}

bool isModeLimit(const Command &cmd)
{
	std::string flag = getFlag(cmd);
	return flag == MODE_L_ADD || flag == MODE_L_REM || flag == MODE_L;
}

#endif // MODE_HPP