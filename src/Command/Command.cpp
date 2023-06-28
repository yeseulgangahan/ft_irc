#include "../../include/Command.hpp"
#include "../../include/CmdManager.hpp"

//TOPIC #ch1 :good~
Command::Command(const std::string& wholeString) : _wholeString(wholeString)
{
    size_t pos = wholeString.find(" :");
	if (pos != std::string::npos)
        _hasTrail = true;
    else
        _hasTrail = false;

    std::vector<std::string> tmpVector = ftSplit(wholeString.substr(0, pos), " ");//트레일링 전까지만 스페이스로 스플릿

    _commandName = tmpVector[0];
    _parameters = std::vector<std::string>(tmpVector.begin() + 1, tmpVector.end());

    if (_hasTrail)
        _trail = wholeString.substr(pos + 2);// 첫 : 다음부터는 트레일링으로 취급
}

bool Command::hasTrailing(void) const { return (_hasTrail); }

std::string Command::getWholeString(void) const
{
    return (_wholeString);
}

void Command::debug(void) const
{
    std::cout << "_commandName: " << _commandName << std::endl;
    std::cout << "_parameters: ";
    
	for(size_t i(0); i < _parameters.size(); ++i)
        std::cout << "<" << _parameters[i] << "> ";
	std::cout<<std::endl;
    
	std::cout << "_trail: " << _trail << std::endl;
}