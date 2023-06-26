#include "../include/Message.hpp"

void reply(const Client &receiver, const std::string &message)
{
	std::string messageCRLF = message + "\r\n";
	send(receiver.getClientFd(), messageCRLF.c_str(), messageCRLF.length(), 0);
	
	std::cout << BLUE << "To Client " << receiver.getNick() << " " << RESET << messageCRLF <<std::endl;
}

void sendWelcomeMessages(const Client &receiver)
{
	reply(receiver, RPL_WELCOME(receiver));
	reply(receiver, RPL_YOURHOST(receiver));
	//reply(receiver, RPL_CREATED(receiver));
	//reply(receiver, RPL_MYINFO(receiver));
}

