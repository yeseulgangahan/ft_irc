// #include "../../../include/CmdManager.hpp"

// void CmdManager::names_all_channel(Client &client)
// {
// 	std::vector<Client> receivers = clientManager.get_connect_clients();
// 	std::set<Client> receivers_set(receivers.begin(), receivers.end());
// 	channelManager.names_all(client, receivers_set);
// }

// void CmdManager::names(Client &client, const Command& cmd)
// {
// 	if (!require_authed(client)) return;
// 	if (!require_nick_user(client)) return;
// 	if (!require_enough_params(client, cmd, 0, 2)) return;
// 	if (cmd._parameters.size() == 0)
// 		names_all_channel(client);
// 	else
// 		channelManager.names_channel(client, cmd);
// }