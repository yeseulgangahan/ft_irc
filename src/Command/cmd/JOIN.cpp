#include "../../../include/CmdManager.hpp"

static std::vector<std::string> parseChannelNames(const Command& cmd)
{
	return ftSplit(cmd._parameters[0], ",");;
}

static std::vector<std::string> parseChannelPasswords(const Command& cmd, const std::vector<std::string> &ChannelNames)
{
	if (cmd._parameters.size() == 1)
		return std::vector<std::string>(ChannelNames.size(), "");

	std::vector<std::string> ch_pwds = ftSplit(cmd._parameters[1], ",");
	if (ch_pwds.size() < ChannelNames.size())
		ch_pwds.resize(ChannelNames.size());
	
	return (ch_pwds);
}

void CmdManager::join(Client &client, const Command& cmd)
{
	if (!requireRegistrationDone(client)) return;
	if (!requireEnoughParams(client, cmd, 1, 3)) return;
	
	std::vector<std::string> ChannelNames = parseChannelNames(cmd);
	std::vector<std::string> ChannelPasswords = parseChannelPasswords(cmd, ChannelNames);

	channelManager.addClient(cmd, client, ChannelNames, ChannelPasswords);
}