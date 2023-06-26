#include "../../include/CmdManager.hpp"

static std::string cutFrontSpace(std::string s)
{
	while (s.size() > 0 && s[0] == ' ')
		s.erase(s.begin());
	return s;
}

static std::string cutFrontCRLF(std::string s)
{
	while (s.length())
	{
		if (s[0] == ' ' || s[0] == '\n')
			s.erase(s.begin());
		else if (s.length() >= 2 && !s.compare(0, 2, "\r\n"))
			s.erase(s.begin(), s.begin() + 2);
		else
			break;
	}

	return (s);
}
//:<source> command parameters
std::string CmdBuffer::trimSource(std::string s)
{
	s = cutFrontSpace(s);
	
	if (s.length() == 0)
		return (s);

	if (s[0] == ':')
	{
		size_t spacePos = s.find(' ');
		if (spacePos == std::string::npos)
			return ("");
		
		size_t commandBegin = spacePos;
		while (commandBegin < s.size() && s[commandBegin] == ' ')
			commandBegin++;

		return (s.substr(commandBegin));
	}

	return (s);
}

void CmdBuffer::appendToBuf(std::string s)
{
	_commandBuffer += s;
	_commandBuffer = cutFrontCRLF(_commandBuffer);
}

bool CmdBuffer::hasCommand(void)
{
	return (_commandBuffer.find("\n") != std::string::npos);
}

Command CmdBuffer::makeCommand(void)
{
	size_t curCommandEnd;
	size_t nextCommandStart;

	if ((curCommandEnd = _commandBuffer.find("\r\n")) != std::string::npos)
		nextCommandStart = curCommandEnd + 2;
	else if ((curCommandEnd = _commandBuffer.find("\n")) != std::string::npos)
		nextCommandStart = curCommandEnd + 1;
	else
		throw std::runtime_error("Error: in commandBuffer");

	std::string wholeString = trimSource(_commandBuffer.substr(0, curCommandEnd));
	_commandBuffer = cutFrontCRLF(_commandBuffer.substr(nextCommandStart));

	return Command(wholeString);
}
