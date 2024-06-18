/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julboyer <julboyer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:04:23 by anbourge          #+#    #+#             */
/*   Updated: 2023/12/08 13:18:31 by julboyer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Commands.hpp"
#include <errno.h>
#include <utility>
#include <functional>

# define DEBUG 1

//Constructors/Destructors

Server::Server() {
	if (DEBUG)
		std::cout << "Server created." << std::endl;
};

Server::~Server() {
	close(_sockfd);
	if (DEBUG)
		std::cout << "Server destroyed." << std::endl;
};

// Methods : Getters
		
int		Server::getSfd(void) const {
	return(this->_sockfd);
};

UserMapFd& Server::getUsersFd() {
	return (this->_usersFd);
}

const UserMapNick& Server::getUsersNick() const {
	return (this->_usersNick);
}

const ChannelMap&	Server::getChannels() const {
	return (_channels);
}

// Methods : Setters

// Methods : Others

void	Server::addUser(int fd, std::string nickname) {
	this->_usersNick.insert(std::make_pair(nickname, fd));
}

void	Server::removeUser(int fd) {
	//delete user object ?
	_usersNick.erase(_usersFd.find(fd)->second.getNickname());
	_usersFd.erase(fd);
}

void	Server::reNicknameUser(User& user, std::string newname) {
	this->_usersNick.erase(user.getNickname());
	user.setNickname(newname);
	this->_usersNick.insert(std::pair<std::string, int>(newname, user.getFd()));
}

int		Server::getUserFd(std::string nickname) {
	return (this->getUsersNick().find(nickname)->second);
};

void	Server::addChannel(std::string name) {
	Channel	c(name);
	_channels.insert(std::pair<std::string, Channel>(name, c));
};

void	Server::removeUserFromChannel(int fd, std::string channel) {
	this->_channels.find(channel)->second.kickUser(&this->getUsersFd().find(fd)->second);
};

void	Server::removeChannel(std::string channel) {
	this->_channels.erase(channel);
	// channel.~Channel(); //enough ?
}
/*
void	Server::renameChannel(Channel& channel, std::string newname) {
	if (!_channels.count(newname))
		channel.setName(newname); //Correctly handled by maps ?
}*/

// Socket functionalities

void	Server::addPort(char *p) {
	this->_port = p;
};

void	Server::addUserBuf(std::string buf, int fd) {
	this->getUsersFd().find(fd)->second.addBuf(buf);
};

std::string	Server::getUserBuf(int fd) {
	return (this->getUsersFd().find(fd)->second.getBuf());
};

void	Server::clearUserBuf(int fd) {
	this->getUsersFd().find(fd)->second.delBuf();
};

void	Server::createSocket(void) {
	const int			enable = 1;
	
	if ((this->_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Error setting up socket (1).\n";
		return ;
	}
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		std::cerr << "Error setting up socket (2).\n";
	}
	this->_sockaddr.sin_family = AF_INET;
	this->_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->_sockaddr.sin_port = htons(atoi(this->_port));
	if ((bind(this->_sockfd, (struct sockaddr *)&this->_sockaddr, sizeof(sockaddr))) < 0) {
		std::cerr << "Error with bind().\n" << errno << std::endl;
	}
	// std::cout << "Socket bound.\n";
};

void	Server::listenSocket(void) {
	//set_nonblocking(this->_sockfd);
	if ((listen(this->_sockfd, SOMAXCONN)) < 0) {
		std::cerr << "Error with listen().\n";
	}
	if (fcntl(this->_sockfd, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << "Error with fcntl().\n";
	}
	// std::cout << "Socket listening.\n";
};

void	Server::createEpoll(void) {
	this->_epollfd = epoll_create1(0);
	if (this->_epollfd < 0) {
		std::cerr << "Error with epoll_create1().\n";
		return;
	}
	this->_epollev.events = EPOLLIN | EPOLLET;
	this->_epollev.data.fd = this->_sockfd;
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_sockfd, &this->_epollev) < 0) {
		std::cerr << "Error with epoll_ctl().\n";
		return;
	}
	this->_numUsers = 0;
};

void	Server::serverConnection(void) try {
	int					n;
	char 				buf[BUFSIZ];
	
	n = epoll_wait(this->_epollfd, this->_evlist, 1024, -1);
	// std::cout << "ServerConnection " << n << std::endl;
	if (n < 0) {
		std::cerr << "Error with epoll_wait().\n";
		return;
	}
	for (int i = 0; i < n; i++) {
		if (this->_evlist[i].data.fd == this->_sockfd) {
			this->firstConnection();
			// std::cout << "End firstConnection" << std::endl;
        }
		if ((this->_evlist[i].events & EPOLLERR) || (this->_evlist[i].events & EPOLLHUP) || (!(this->_evlist[i].events & EPOLLIN))) {
			if (epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, this->_evlist[i].data.fd, &this->_evlist[i]) == -1) {
				std::cerr << "Error with epoll_ctl().\n";
				return;
			}
			close(this->_evlist[i].data.fd);
        	if (this->_evlist[i].data.fd == -1) {
				std::cerr << "Error with close().\n";
				return;
			}
			std::cout << "\x1b[0;31m[*] close\x1b[0m\n";	
		}
        else
			this->ClientRequestHandler(buf, this->_evlist[i].data.fd);
	}
}
catch (const std::exception& e)
{
	std::cerr << "serverConnexion " << e.what() << " " <<"\n";
}

int		Server::firstConnection() try {
	int						cfd;
	struct sockaddr_in		csin;
	std::vector<Message>	msgs;
	User					nusr;

	// accept
	size_t	addrlen = sizeof(struct sockaddr_in);
    cfd = accept(this->_sockfd, (struct sockaddr *)&csin, (socklen_t *)&addrlen);
    if (cfd == -1) {
		std::cerr << "Error with accept().\n";
		return (-1);
	}
	nusr.setFd(cfd);
    std::cout << "\x1b[0;32m[*] accept\x1b[0m\n";
	if (fcntl(cfd, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << "Error with fcntl().\n";
	}
    this->_epollev.events = EPOLLIN | EPOLLET;
    this->_epollev.data.fd = cfd;
    if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, cfd, &this->_epollev) == -1) {
		std::cerr << "Error with epoll_ctl().\n";
		return (-1);
	}
    //std::cout << "first command : " << buf << " TEST " << std::endl;
	//std::cout << "TMPBUF : \"" << this->getTmpBuf().c_str() << "\"" << std::endl;
	this->_usersFd.insert(std::make_pair(nusr.getFd(), nusr));
	// std::cout << "return (cfd) 303" << std::endl;
	return (cfd);
}
catch (const std::exception& e)
{
	std::cerr << e.what() << " " <<"\n";
	return (-1);
}

void	Server::ClientRequestHandler(char *buf, int fd) {
    int 				buflen;
	std::string			tmp;
	t_cmd				cmd;
	
    // read
    buflen = read(fd, buf, BUFSIZ-1);
	if (buflen <= 0)
		return ;
	buf[buflen] = '\0';
	tmp = buf;
	// std::cout << "BUF = " << buf << std::endl;
	if (tmp.find("\r\n") == std::string::npos) {
		this->addUserBuf(tmp, fd);
		return ;
	}
	this->addUserBuf(tmp, fd);
	// std::cout << "TMP = " << tmp << std::endl;
	tmp.clear();
	// std::cout << "BUFFER : " << buf << " buflen : " << buflen << std::endl;
	std::string str = this->getUserBuf(fd);
	// std::cout << "STR : " << str << " buflen : " << buflen << std::endl;
	std::vector<Message>	msg = this->parser(str, &(this->_usersFd.find(1)->second), &(this->_usersFd.find(0)->second));
	ptmf	pcmd;
	for (unsigned int i = 0; i < msg.size(); i++) {
		// std::cout << "msg[i].getCommand() : " << msg[i].getCommand() << std::endl;
  		pcmd = getCommands(msg[i]);
		t_cmd	cmd;
		if (pcmd)
		{
			cmd.msg = msg[i];
			cmd.serv = this;
			if (this->getUsersFd().find(fd) != this->getUsersFd().end())
				cmd.user = &this->getUsersFd().find(fd)->second;
			else
				return ;
			// std::cout << "COMMAND : " << msg[i].getCommand() << std::endl;
			if (!cmd.user->isRegistered() && (msg[i].getCommand().find("PASS") == std::string::npos && msg[i].getCommand().find("NICK") == std::string::npos && msg[i].getCommand().find("USER") == std::string::npos && msg[i].getCommand().find("CAP LS") == std::string::npos && msg[i].getCommand().find("QUIT") == std::string::npos)) {
				this->clearUserBuf(fd);
				sendErrorMessage("451", ": Client must be registered before the server will allow it to be parsed in detail.", cmd.user->getNickname(), cmd.user->getFd());
				return ;
			}
			(*pcmd)(cmd);
			if (this->getUsersFd().find(fd) != this->getUsersFd().end() && !cmd.user->isRegistered() && cmd.user->getNickname().size() && cmd.user->getRealname().size())
				cmd.user->finishRegistration();
		}
		//std::cout << "received message = " << this->getUserBuf(fd) << std::endl;
	}
	if (this->getUsersFd().find(fd) != this->getUsersFd().end())
		this->clearUserBuf(fd);
}

//server = 0, client = 1
std::vector<Message>		Server::parser(std::string str, User *sender, User *receiver) {
	std::vector<Message>		ret;
	size_t			pos, pos2, i;
	std::string	cpy = str;

	i = 0;
	pos2 = 0;
	while (pos2 != cpy.npos) {
		ret.push_back(Message());
		pos = cpy.find(" ");
		//std::cout << "POS = " << pos << std::endl;
		ret[i].setCommand(cpy.substr(0, pos));
		pos2 = cpy.find("\n");
		//std::cout << "POS2 = " << pos2 << std::endl;
		ret[i].setArgs(cpy.substr(pos + 1, pos2 - pos - 2));
		cpy.erase(0, pos2 + 1);
		ret[i].setSender(sender);
		ret[i].setReceiver(receiver);
		ret[i].setEnd(false);
		i++;
		pos2 = cpy.find("\n");
		//std::cout << "POS2 = " << pos2 << " NPOS = " << cpy.npos << std::endl;
	}
	ret[i - 1].setEnd(true);
	//std::cout << "COMMAND = " << ret[i - 1].getCommand() << std::endl;
	//std::cout << "ARGS = " << ret[i - 1].getArgs() << std::endl;
	return (ret);
};

void	Server::addPw(char *str) {
	this->_pw.assign(str);
};

std::string	Server::getPw(void) {
	return (this->_pw);
};

void	Server::setServerName(std::string name) {
	this->_server_name = name;
};

std::string	Server::getServerName(void) {
	return (this->_server_name);
};

void	Server::changeChannelTopic(std::string chan, std::string topic) {
	this->_channels.find(chan)->second.setTopic(topic);
}

void	Server::addUsertoChannel(User *usr, std::string ChanName) {
	this->_channels.find(ChanName)->second.addUser(usr);
}

void	Server::setInviteChannel(std::string channel, bool option) {
	this->_channels.find(channel)->second.setInvite(option);
};

void	Server::setTopicOpChannel(std::string channel, bool option) {
	this->_channels.find(channel)->second.setTopicOp(option);
};

void	Server::setKeyChannel(std::string channel, std::string key) {
	this->_channels.find(channel)->second.setKey(key);
};

void	Server::removeKeyChannel(std::string channel) {
	this->_channels.find(channel)->second.removeKey();
};

void	Server::setLimitChannel(std::string channel, int limit) {
	this->_channels.find(channel)->second.setLimit(limit);
};

void	Server::removeLimitChannel(std::string channel) {
	this->_channels.find(channel)->second.removeLimit();
};
