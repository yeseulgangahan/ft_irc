#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include "Client.hpp"
#include "Message.hpp"
#include "Command.hpp"

typedef std::set<Client>::iterator client_it;

class Channel
{
private:
    
    //Basic
    std::string _name; 
    std::string _password;
    std::string _topic;
    size_t _limitNum;
    
    //User List
    std::set<Client> _members;
    std::set<Client> _operators;
    std::set<Client> _invited;

    //MODE
    bool _modeInvite;
    bool _modeTopic;
    bool _modeLimit;
    bool _modeKey;

    //변형된 결과 getter (private)
    bool isOperator(const Client& target) const;
    std::string getTopicReplyString(const Client& sender);

    // 체크하고 에러 답장
    bool requireOperator(Client& sender) const;
    bool requireSenderOnChannel(const Client& sender) const;
    bool requireTargetInChannel(Client& sender, Client &target) const;
    
public:
    //생성자
    Channel(const Command& cmd, const std::string &name, const Client& client, const std::string &pwd);
    
    //기본 getter
    std::string getName() const;
    const std::set<Client>& getMembers() const;

    //변형된 결과 getter
    std::string getModeString();
    std::string getMembershipString(const Client&client) const;
    bool isMember(const Client& target) const;
    
    //내용 변경
    void removeMember(Client& target);
    void addMember(const Command&cmd, Client& sender, const std::string & pass = "");
    void ejectMember(const Command&cmd, Client &sender, Client& target);
    void setTopic(const Command&cmd, Client &sender, const std::string &topic);
    void addInvitedList(const Command&cmd, Client &sender, Client& target);

    //클라이언트에게 행동
    void broadcast(const Client& sender, std::string message) const;
    void broadcastExceptSender(const Client& sender, std::string message) const;
    void replyNamesCommend(const Client& sender) const;
    void showTopic(Client &sender);
    
    void modeInviteState(Client &client);
    void modeInvite(const Command&cmd, Client &sender, bool valid);
    void modeOperator(const Command&cmd, Client &sender, bool valid, Client &target);
    void modeTopicState(Client &sender);
    void modeTopic(const Command&cmd, Client &sender, bool valid);
    void modeKey(Client &sender);
    void modeKeyAdd(const Command&cmd, Client &sender, const std::string &new_pass);
    void modeKeyRemove(const Command&cmd, Client &sender);
    void modeLimitState(Client &sender);
    void modeLimitAdd(const Command&cmd, Client &sender, const std::string &new_pass);
    void modeLimitRemove(const Command&cmd, Client &sender);

    // 삭제??
    bool operator<(const Channel& rhs) const;
};

#endif

