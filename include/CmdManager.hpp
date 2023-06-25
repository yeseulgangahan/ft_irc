#ifndef CMD_MANAGER_HPP
# define CMD_MANAGER_HPP

# include "ClientManager.hpp"
# include "ChannelManager.hpp"
 
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

class CmdManager
{
	private:
		ClientManager &clientManager;
		ChannelManager &channelManager;
		std::string _serverPassword;
		
		void mode_i(Channel &channel, Client&sender, const Command &cmd);
		void mode_o(Channel &channel, Client&sender, const Command &cmd);
		void mode_t(Channel &channel, Client&sender, const Command &cmd);
		void mode_k(Channel &channel, Client&sender, const Command &cmd);
		void mode_l(Channel &channel, Client&sender, const Command &cmd);
		void mode_state(Channel &channel, Client&sender);
		//void names_all_channel(Client &client);
		void send_quit_msg(Client&sender, const Command &cmd);
	
	public:
		CmdManager(ClientManager &clientManager, ChannelManager &channelManager, const std::string &serverPassword);
		
		void join(Client &client, const Command &cmd);
		
		bool require_enough_params(Client &sender, const Command& cmd, size_t ok_min, size_t ng_min = SIZE_MAX, bool require_trailing = false);
		
		//void hangup_quit(Client&sender);
		std::vector<Command> parse_commands(const std::string &commands_msg);
		void executeCommand(Client &sender, const Command &cmd);
		void cap(Client &client, const Command &cmd);
		void pass(Client &client, const Command& cmd);
		void nick(Client &client, const Command& cmd);
		void user(Client &client, const Command &cmd);
		//void cap(Client &client);
		void quit(Client&client, const Command &cmd);
		//void names(Client &client, const Command& cmd);
		void mode(Client &client, const Command& cmd);
		//void PONG(Client &client, std::vector<std::string> &server_name);
		void privmsg(Client& client, const Command &cmd);
		void kick(Client &client, const Command& cmd);
		void invite(Client& sender, const Command& cmd);
		void part(Client& sender, const Command& cmd);
		void topic(Client& sender, const Command& cmd);
		void ping(Client &sender, const Command& cmd);
		void who(Client &client, const Command& cmd);
		void notice(Client &client, const Command& cmd);

		bool require_authed(Client &client);
		bool require_nick_user(Client &client);

		// std::vector<std::string> ft_split(const std::string& s, const std::vector<std::string>& t);
		// std::vector<std::string> ft_split(const std::string& s, const std::string& t);
		// std::vector<std::string> splitByLines(const std::string& s);
};

std::vector<std::string> ft_split(const std::string& s, const std::vector<std::string>& t);
std::vector<std::string> ft_split(const std::string& s, const std::string& t);
std::vector<std::string> splitByLines(const std::string& s);

#endif