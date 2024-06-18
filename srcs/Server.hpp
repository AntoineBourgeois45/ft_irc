/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anbourge <anbourge@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:04:30 by anbourge          #+#    #+#             */
/*   Updated: 2023/07/05 16:28:12 by anbourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <iostream>
# include <netdb.h>
# include <string>
# include <cstring>
# include <cstdlib>
# include <cstdio>
# include <poll.h>
# include <vector>
# include <sys/epoll.h>
# include <map>
# include <ctime>
# include <fcntl.h>
# include "User.hpp"
# include "Channel.hpp"
# include "Message.hpp"
# include "User.hpp"

typedef std::map<int, User> UserMapFd;
typedef std::map<const std::string, int> UserMapNick;
typedef std::map<std::string, Channel> ChannelMap;

class Server {
	public:

		Server();
		~Server();

		int					getSfd(void) const;

		void				addPort(char *p);
		void				createSocket(void);
		void				listenSocket(void);
		void				serverConnection(void);
		void				createEpoll(void);

		void				ClientRequestHandler(char *buf, int fd);
		int					firstConnection();

		std::vector<Message>				parser(std::string str, User	*sender, User *receiver);

		void				addPw(char *str);
		std::string			getPw(void);

		void				addUserBuf(std::string buf, int fd);
		std::string			getUserBuf(int fd);
		void				clearUserBuf(int fd);

		std::string			getServerName(void);
		void				setServerName(std::string name);

		UserMapFd&			getUsersFd();
		const UserMapNick&	getUsersNick() const;
		void				addUser(int fd, std::string nickname);
		void				removeUser(int fd);
		void				reNicknameUser(User& user, std::string newname);
		int					getUserFd(std::string nickname);

		const ChannelMap&	getChannels() const;
		void				addChannel(std::string name);
		void				removeUserFromChannel(int fd, std::string channel);
		void				changeChannelTopic(std::string chan, std::string topic);
		void				addUsertoChannel(User *usr, std::string ChanName);
		void				removeChannel(std::string channel);

		void				setInviteChannel(std::string channel, bool option);
		void				setTopicOpChannel(std::string channel, bool option);
		void				setKeyChannel(std::string channel, std::string key);
		void				removeKeyChannel(std::string channel);
		void				setLimitChannel(std::string channel, int limit);
		void				removeLimitChannel(std::string channel);

	private:

		std::string			_server_name;

		std::string			_pw;
		int					_sockfd;
		char				*_port;
		sockaddr_in			_sockaddr;

		std::string			_buftmp;

		struct epoll_event 	_epollev;
		int					_epollfd;
		struct epoll_event 	_evlist[1024];

		int					_numUsers;

		UserMapFd 			_usersFd;
		UserMapNick			_usersNick;
		ChannelMap			_channels;
};


#endif