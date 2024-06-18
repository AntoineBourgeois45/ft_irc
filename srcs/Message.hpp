#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <sys/types.h>
# include <cerrno>
# include "User.hpp"
# include "Channel.hpp"

class Message	{
	private :
		User		*_sender;
		User		*_receiver;
		std::string	_command;
		std::string	_args;
		bool		_end;

	public :
		Message(void);
		User		*getSender(void);
		User		*getReceiver(void);
		std::string	getCommand(void);
		std::string	getArgs(void);
		bool		getEnd(void);

		void		setCommand(std::string str);
		void		setArgs(std::string str);
		void		setSender(User *i);
		void		setReceiver(User *i);
		void		setEnd(bool b);
};

int			sendMessage(std::string message, int fd);
int			sendErrorMessage(std::string code, std::string content, std::string nick, int fd);
int			sendGlobalMessage(std::string message, const Channel Chan, int sender);
int			sendGlobalErrorMessage(std::string code, std::string content, const Channel chan);

#endif