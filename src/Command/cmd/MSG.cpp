#include "../../../include/CmdManager.hpp"

enum e_reply_require{recv_reply, not_recv_reply};
static bool require_valid_param(Client &client, const Command &cmd, CmdManager &cmdManager, e_reply_require reply_require)
{
	bool is_ok = true;
	if (cmd._parameters.size() == 0)
	{
		if(reply_require == recv_reply)
			client.appendToSendBuffer(ERR_NORECIPIENT(client, cmd.getWholeString()));
		is_ok = false;
	}
	else if (!cmdManager.requireEnoughParams(client, cmd, 1, 2))
	{
		is_ok = false;
	}
	if (!cmd.hasTrailing())
	{
		if(reply_require == recv_reply)
			client.appendToSendBuffer(ERR_NOTEXTTOSEND(client));
		is_ok = false;	
	}
	return is_ok;
}

static std::vector<std::string> parse_targets(const Command &cmd)
{
	return ftSplit(cmd._parameters[0], ",");
}

static bool is_channel(const std::string &reicieve)
{
	return reicieve != "" && reicieve[0] == '#';
}

void CmdManager::privmsg(Client& client, const Command &cmd)
{
	if (!requireAuthed(client)) return;
	if (!requireNickUser(client)) return;
	if (!require_valid_param(client, cmd, *this, recv_reply)) return;
	
	std::vector<std::string> targets = parse_targets(cmd);
	for (size_t i = 0; i < targets.size(); i++)
	{
		if (is_channel(targets[i]))
			channelManager.broadcastToChannel(cmd, client, targets[i]);
		else
			clientManager.privmsg(cmd, client, targets[i]);
	}
}

void CmdManager::notice(Client &sender, const Command& cmd)
{
	if (!sender.isAuthed()) return;
	if (!sender.isUserSetted() || !sender.isNicknameSetted()) return;
	if (!require_valid_param(sender, cmd, *this, not_recv_reply)) return;

	std::vector<std::string> targets = parse_targets(cmd);

	for (size_t i = 0; i < targets.size(); i++)
	{
		if (is_channel(targets[i]))
			channelManager.broadcastToChannel(cmd, sender, targets[i]);
		else
			clientManager.notice(cmd, sender, targets[i]);
	}
}