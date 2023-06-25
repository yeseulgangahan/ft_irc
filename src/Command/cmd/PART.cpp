#include "../../../include/CmdManager.hpp"

static std::vector<std::string> parseChannelNames(const Command& cmd)
{
	return ft_split(cmd._parameters[0], ",");;
}

//PART #a
void CmdManager::part(Client& sender, const Command& cmd)
{
	if (!require_authed(sender)) return;
	if (!require_nick_user(sender)) return;
	if (!require_enough_params(sender, cmd, 1, 2))	return;
	std::vector<std::string> ch_names = parseChannelNames(cmd);
	channelManager.part_list(cmd, sender, ch_names);
}