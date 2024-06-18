#include "User.hpp"
#include "Message.hpp"

// Constructors

User::User() {
	_userbuf = "";
	_nickname = "";
	_registered = false;
	_passed = false;
}

// Destructors

User::~User() {
}

// Methods : Getters

const std::string&	User::getNickname() const {
	return (this->_nickname);
}

const std::string&	User::getUsername() const {
	return (this->_username);
}

const std::string&	User::getRealname() const {
	return (this->_realname);
}

const std::string&	User::getHostname() const {
	return (this->_hostname);
}

const std::string&	User::getServername() const {
	return (this->_servername);
}

const int&	User::getFd() const {
	return (this->_fd);
}

const JoinedChanMap&	User::getJoinedchannels() const {
	return (this->_joinedchannels);
}

const InvitedChanMap&	User::getInvitedchannels() const {
	return (this->_invitedchannels);
};

// Methods : Setters

void	User::setNickname(std::string nickname) {
	this->_nickname = nickname;
}

void	User::setUsername(std::string username) {
	this->_username = username;
}

void	User::setRealname(std::string realname) {
	this->_realname = realname;
}

void	User::setHostname(std::string hostname) {
	this->_hostname = hostname;
}

void	User::setServername(std::string servername) {
	this->_servername = servername;
}

void	User::setFd(int fd) {
	this->_fd = fd;
}

void	User::addJoinedChannel(std::string channel, bool op) {
	this->_joinedchannels.insert(std::pair<std::string, bool>(channel, op));
};

void	User::delJoinedChannel(std::string channel) {
	this->_joinedchannels.erase(channel);
};

void	User::addInvitedChannel(std::string channel) {
	this->_invitedchannels.insert(std::pair<std::string, bool>(channel, true));
};

void	User::delInvitedChannel(std::string channel) {
	this->_invitedchannels.erase(channel);
};

bool	User::isRegistered() const {
	return (_registered);
};

void	User::setRegistered(bool val) {
	this->_registered = val;
};

void					User::finishRegistration() {
	sendMessage(":test.server.com 001 " + this->getNickname() + " :Welcome to the Internet Relay Network " + this->getNickname() + "!" + this->getUsername() + "@localhost\r\n" , this->_fd);
	sendMessage(":test.server.com 002 " + this->getNickname() + " :Your host is test.server.com, running version 0.1\r\n" , this->_fd);
	time_t timestamp = time(NULL);
	sendMessage(":test.server.com 003 " + this->getNickname() + " :This server was created " + ctime(&timestamp) + "\r\n" , this->_fd);
	sendMessage(":test.server.com 004 " + this->getNickname() + " test.server.com 0.1\r\n" , this->_fd);
	sendMessage(":test.server.com 005 " + this->getNickname() + " CHANTYPES=# CHANMODES=itkol CASEMAPPING=ascii CHANNELLEN=50 RFC2812 MODES=3 :are supported by this server\r\n" , this->_fd);
	this->_registered = true;
};

void					User::setPassed(bool val) {
	this->_passed = val;
};


const bool				&User::getPassed() const {
	return (this->_passed);
};

void					User::addBuf(std::string buf) {
	this->_userbuf += buf;
};

void					User::delBuf() {
	this->_userbuf.clear();
};

std::string				User::getBuf() {
	return (this->_userbuf);
};
