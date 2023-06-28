// #include "../../../include/CmdHandler.hpp"

// void CmdHandler::names_all_channel(Client &client)
// {
// 	std::vector<Client> receivers = clientHandler.getConnectClients();
// 	std::set<Client> receivers_set(receivers.begin(), receivers.end());
// 	channelHandler.names_all(client, receivers_set);
// }

// void CmdHandler::names(Client &client, const Command& cmd)
// {
// 	if (!requireAuthed(client)) return;
// 	if (!requireNickUser(client)) return;
// 	if (!requireEnoughParams(client, cmd, 0, 2)) return;
// 	if (cmd._parameters.size() == 0)
// 		names_all_channel(client);
// 	else
// 		channelHandler.names_channel(client, cmd);
// }