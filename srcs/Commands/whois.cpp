#include "../Commands.hpp"

void	whois(t_cmd cmd) { //Not a required command, irssi sends it to know which user got a desired nickname
	User &usr = cmd.serv->getUsersFd().find(cmd.serv->getUsersNick().find(cmd.msg.getArgs())->second)->second;
	sendErrorMessage("311",usr.getNickname() + " " + usr.getUsername() + " " + usr.getHostname() + " * :" + usr.getRealname(), cmd.user->getNickname(), cmd.user->getFd());
	sendErrorMessage("312", usr.getNickname() + " " + cmd.serv->getServerName() + " :", cmd.user->getNickname(), cmd.user->getFd());
	sendErrorMessage("318", cmd.msg.getArgs() + " :End of WHOIS list", cmd.user->getNickname(), cmd.user->getFd());
}