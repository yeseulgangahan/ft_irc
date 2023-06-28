// #include "../../../include/CmdManager.hpp"

// void CmdManager::names_all_channel(Client &client)
// {
// 	std::vector<Client> receivers = clientManager.getConnectClients();
// 	std::set<Client> receivers_set(receivers.begin(), receivers.end());
// 	channelManager.names_all(client, receivers_set);
// }

// void CmdManager::names(Client &client, const Command& cmd)
// {
// 	if (!requireAuthed(client)) return;
// 	if (!requireRegistrationDone(client)) return;
// 	if (!requireEnoughParams(client, cmd, 0, 2)) return;
// 	if (cmd._parameters.size() == 0)
// 		names_all_channel(client);
// 	else
// 		channelManager.names_channel(client, cmd);
// }