# ifndef COMMAND_H
# define COMMAND_H

# include <iostream>
# include <algorithm>
# include <sys/socket.h> 
# include <sys/types.h>
# include <arpa/inet.h>
# include <unistd.h> 
# include <string> 
# include <fcntl.h>
# include <signal.h>
# include <sys/poll.h>
# include <vector>
# include <string>
# include <algorithm>
# include <map>
# include <stdexcept>


//COMMAND [PARAMETER1] [PARAMETER2] ... [:TRAILING]

class CmdHandler;
class Command
{
	private:
	    std::string _wholeString;
	    bool _hasTrail;
	
	public:
	    std::string _commandName;
		std::vector<std::string> _parameters;
	    std::string _trail;  

	    Command(const std::string& wholeString);
	    std::string getWholeString(void) const;
	    bool hasTrailing(void) const;
	    void debug(void) const;
};

#endif