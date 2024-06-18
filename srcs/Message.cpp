#include "Message.hpp"

Message::Message() : _sender(NULL), _receiver(NULL), _command(), _args()
{
}

User			*Message::getSender(void) {
	return (this->_sender);
};

User			*Message::getReceiver(void) {
	return (this->_receiver);
};

std::string	Message::getCommand(void) {
	return (this->_command);
};

std::string	Message::getArgs(void) {
	return (this->_args);
};

bool		Message::getEnd(void) {
	return (this->_end);
};

void		Message::setCommand(std::string str) {
	this->_command = str;
};

void		Message::setArgs(std::string str) {
	this->_args = str;
};

void		Message::setSender(User *i) {
	this->_sender = i;
};

void		Message::setReceiver(User *i) {
	this->_receiver = i;
};

void		Message::setEnd(bool b) {
	this->_end = b;
};

int		sendMessage(std::string message, int fd) {
	int ret = send(fd, message.c_str(), message.size(), MSG_NOSIGNAL);
	std::cout << "Message sent :\n" << message << std::endl;
	if (ret == -1) {
		std::cerr << "Error with send().\n";
		std::cerr << errno << std::endl;
		return -1;
	}
	return 0;
};

int		sendGlobalMessage(std::string message, const Channel chan, int sender) //sends message to every user except for sender
{
	UserpointMapFd					users;
	UserpointMapFd::const_iterator	itb;
	UserpointMapFd::const_iterator	ite;
	int 							ret = 0;

	users = chan.getUserMap();
	itb = users.begin();
	ite = users.end();
	std::cout << "Sending global message on " << chan.getName() << std::endl;
	while (itb != ite) {
		if (itb->second->getFd() != sender)
			ret = send(itb->second->getFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
		if (ret == -1) {
			std::cerr << "Error with send().\n";
			std::cerr << errno << std::endl;
			return -1;
		}
		itb++;
	}
	std::cout << "GLOBAL MESSAGE SENT : " << message << std::endl;
	return 0;
}

int		sendErrorMessage(std::string code, std::string content, std::string nick, int fd) { //server name usage a modifier
	std::string message;
	message = ":test.server.com " + code + " " + nick + " " + content + "\r\n";
	int ret = send(fd, message.c_str(), message.size(), MSG_NOSIGNAL);
	std::cout << "Message sent :\n" << message << std::endl;
	if (ret == -1) {
		std::cerr << "Error with send().\n";
		std::cerr << errno << std::endl;
		return -1;
	}
	return 0;
};

//same function as above exept it sends numeric reply to every user in a channel, used by topic to let everyone knows of a topic change
int		sendGlobalErrorMessage(std::string code, std::string content, const Channel chan) { 
	std::string message;
	UserpointMapFd					users;
	UserpointMapFd::const_iterator	itb;
	UserpointMapFd::const_iterator	ite;
	int ret;

	users = chan.getUserMap();
	itb = users.begin();
	ite = users.end();
	while (itb != ite)
	{
		message = ":test.server.com " + code + " " + itb->second->getNickname() + " " + content + "\r\n";
		ret = send(itb->second->getFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
		std::cout << "Message sent :\n" << message << std::endl;
		if (ret == -1) {
			std::cerr << "Error with send().\n";
			std::cerr << errno << std::endl;
			return -1;
		}
		itb++;
	}
	return 0;
};