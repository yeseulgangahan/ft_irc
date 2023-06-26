#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
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

    //*** ?? getter로 대체 후 삭제
    bool requireValidNum(Client &sender, const std::string &string);//mode i 에서만 사용

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
    const std::set<Client>& get_members() const;//*** getMembers()

    //변형된 결과 getter
    std::string get_mode() const;// getModeString()
    std::string getMembershipString(const Client&client) const;
    bool is_member(const Client& target) const;//isMember()
    
    //내용 변경
    void removeClient(Client& target);
    void addMember(const Command&cmd, Client& sender, const std::string & pass = "");
    void set_topic(const Command&cmd, Client &sender, const std::string &topic); // ***setTopic

    //클라이언트에게 행동
    void broadcast(const Client& sender, std::string message) const;
    void broadcastExceptSender(const Client& sender, std::string message) const;
    void names(const Client& sender) const;//*** replyNamesCommend()
    void show_topic(Client &sender);//*** showTopic()
    
    // ?? 아직안봄
    void kick(const Command&cmd, Client &sender, Client& target);
    void invite(const Command&cmd, Client &sender, Client& target);

    //*** ?? mode 뺄 때 내용도 삭제하는지 체크할 것
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

