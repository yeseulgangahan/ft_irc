#include "../../../include/CmdHandler.hpp"

static std::vector<std::string> parseChannelNames(const Command& cmd)
{
	return ftSplit(cmd._parameters[0], ",");;
}

//PART #a
void CmdHandler::part(Client& sender, const Command& cmd)
{
<<<<<<< HEAD
	if (!requireRegistrationDone(sender)) return;
	if (!requireEnoughParams(sender, cmd, 1, 2))	return;
=======
	if (!requireAuthed(sender)) return;
	if (!requireNickUser(sender)) return;
	if (!requireEnoughParams(sender, cmd, 1, 2)) return;

>>>>>>> d743227d882f5768e1fc1215b84d1a90b35ffc48
	std::vector<std::string> ch_names = parseChannelNames(cmd);
	channelHandler.removeDepartedClient(cmd, sender, ch_names);
}