#include "Channel.hpp"

// Constructors

Channel::Channel() {
	this->_inviteonly = false;
	this->_topicop = true;
	this->_ulimit = 1024;
	this->_islimit = false;
	this->_unumber = 0;
	this->_iskey = false;
}

Channel::Channel(std::string name) {
	this->_name = name;
	this->_inviteonly = false;
	this->_topicop = true;
	this->_ulimit = 1024;
	this->_islimit = false;
	this->_unumber = 0;
	this->_iskey = false;
}

// Destructors

Channel::~Channel() {
}

// Methods : Getters

const std::string	&Channel::getName() const {
	return (this->_name);
}

const int			&Channel::getLimitNum() const {
	return (this->_ulimit);
};

const bool			&Channel::getLimit() const {
	return (this->_islimit);
}

const int			&Channel::getUserNumber() const {
	return (this->_unumber);
};

const bool			&Channel::getInvite() const {
	return (this->_inviteonly);
};

const std::string	&Channel::getKey() const {
	return (this->_key);
};

const bool			&Channel::getIsKey() const {
	return (this->_iskey);
};

const std::string	&Channel::getTopic() const {
	return (_topic);
}

const bool			&Channel::getTopicOp() const {
	return (this->_topicop);
};

const UserpointMapFd	&Channel::getUserMap() const {
	return (this->_users);
};

// Methods : Setters

void				Channel::setName(std::string name) {
	this->_name = name;
}

void				Channel::setLimit(int limit) {
	this->_ulimit = limit;
	this->_islimit = true;
};	

void				Channel::setInvite(bool inv) {
	this->_inviteonly = inv;
};

void				Channel::setKey(std::string key) {
	this->_key = key;
	this->_iskey = true;
};

void				Channel::setTopic(std::string topic) {
	this->_topic = topic;
}

void				Channel::setTopicOp(bool op) {
	this->_topicop = op;
};

void				Channel::removeKey() {
	this->_key.erase();
	this->_iskey = false;
};

void				Channel::removeLimit() {
	this->_islimit = false;
};

void				Channel::addUser(User *user) {
	this->_users.insert(std::pair<int, User*>(user->getFd(), user));
	this->_unumber = this->_unumber + 1;
};

void				Channel::kickUser(User *user) {
	this->_users.erase(this->_users.find(user->getFd()));
	this->_unumber = this->_unumber - 1;
	user->delJoinedChannel(this->getName());
};
