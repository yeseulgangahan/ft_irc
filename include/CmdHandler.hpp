#ifndef CMD_MANAGER_HPP
# define CMD_MANAGER_HPP

# include "ClientHandler.hpp"
# include "ChannelHandler.hpp"
 
# define CAP  "CAP"
# define  PASS  "PASS"	
# define  NICK  "NICK"	
# define USER  "USER"	
# define  JOIN  "JOIN"	
# define  TOPIC  "TOPIC"	
# define  PING  "PING"	
# define  NAMES  "NAMES"	
# define  MODE  "MODE"	
# define  PRIVMSG  "PRIVMSG"	
# define  QUIT  "QUIT"	
# define  KICK  "KICK"	
# define  INVITE  "INVITE"	
# define  PART  "PART"	
# define  WHO  "WHO"	
# define  NOTICE  "NOTICE"

class CmdHandler
{
	private:
		ClientHandler &clientHandler;
		ChannelHandler &channelHandler;
		std::string _serverPassword;
		
		void modeInvite(Channel &channel, Client&sender, const Command &cmd);
		void modeOperator(Channel &channel, Client&sender, const Command &cmd);
		void modeTopic(Channel &channel, Client&sender, const Command &cmd);
		void modeLimit(Channel &channel, Client&sender, const Command &cmd);
		void modeState(Channel &channel, Client&sender);
		void sendQuitMessage(Client&sender, const Command &cmd);
	
	public:
		CmdHandler(ClientHandler &clientHandler, ChannelHandler &channelHandler, const std::string &serverPassword);
		
		void join(Client &client, const Command &cmd);
		
		bool requireEnoughParams(Client &sender, const Command& cmd, size_t ok_min, size_t ng_min = SIZE_MAX, bool require_trailing = false);
		
		std::vector<Command> parseCommands(const std::string &commands_msg);
		void executeCommand(Client &sender, Command cmd);
		void cap(Client &client, const Command &cmd);
		void pass(Client &client, const Command& cmd);
		void nick(Client &client, const Command& cmd);
		void user(Client &client, const Command &cmd);
		void quit(Client&client, const Command &cmd);
		void mode(Client &client, Command& cmd);
		void privmsg(Client& client, const Command &cmd);
		void kick(Client &client, const Command& cmd);
		void invite(Client& sender, const Command& cmd);
		void part(Client& sender, const Command& cmd);
		void topic(Client& sender, const Command& cmd);
		void ping(Client &sender, const Command& cmd);
		void who(Client &client, const Command& cmd);
		void notice(Client &client, const Command& cmd);

		bool requireAuthed(Client &client);
		bool requireRegistrationDone(Client &client);

		void rmOverlaps(std::string& s);
		void plusOption(Channel &channel, Client &sender, const Command &cmd);
		void minusOption(Channel &channel, Client &sender, const Command &cmd);
};

std::vector<std::string> ftSplit(const std::string& s, const std::vector<std::string>& t);
std::vector<std::string> ftSplit(const std::string& s, const std::string& t);
std::vector<std::string> splitByLines(const std::string& s);

#endif