#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <iostream>
# include <string>
# include <vector>
# include "Message.hpp"
# include "User.hpp"
# include "Server.hpp"

class Server;

typedef	struct		s_cmd {
	Message		msg;
	User		*user;
	Server		*serv;
}					t_cmd;

typedef	void	(*ptmf)(t_cmd);

ptmf	getCommands(Message msg);
void	join(t_cmd cmd);
void	nick(t_cmd cmd);
void	kick(t_cmd cmd);
void	mode(t_cmd cmd);
void	invite(t_cmd cmd);
void	topic(t_cmd cmd);
void	privmsg(t_cmd cmd);
void	pass(t_cmd cmd);
void	user(t_cmd cmd);
void	ping(t_cmd cmd);
void	whois(t_cmd cmd);
void	quit(t_cmd cmd);
void	part(t_cmd cmd);

#endif