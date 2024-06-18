#include "../Commands.hpp"

int isspecial(char c)
{
	if ((c >= '{' && c <= '}') || (c >= '[' && c <= '`'))
		return (1);
	return (0);
}

int		check_nick(std::string nick)
{
	std::string::iterator itb;
	std::string::iterator ite;
	if (nick.size() > 9)
		return (1);
	if ((nick[0] >= '0' && nick[0] <= '9') || nick[0] == '-')
		return (1);
	itb = nick.begin();
	ite = nick.end();
	while (itb != ite) {
		if (!((*itb >= '0' && *itb <= '9') || (*itb >= 'A' && *itb <= 'Z') || (*itb >= 'a' && *itb <= 'z') || *itb != '-' || isspecial(*itb)))
			return (1);
		itb++;
	}
	return (0);
}

void	nick(t_cmd cmd) {
	std::string oldNick;

	oldNick = cmd.user->getNickname();
	
	if (check_nick(cmd.msg.getArgs())) {
		sendErrorMessage("432", cmd.msg.getArgs() + ": Invalid nickname", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	if (cmd.serv->getUsersNick().find(cmd.msg.getArgs()) != cmd.serv->getUsersNick().end()) { //if nickname is already used
		if (cmd.user->getNickname().size())
			sendErrorMessage("433", cmd.msg.getArgs() + ": Nickname already in use", cmd.user->getNickname(), cmd.user->getFd());
		else
			sendErrorMessage("433", cmd.msg.getArgs() + ": Nickname already in use", "*", cmd.user->getFd());
		return ;
	}
	cmd.serv->reNicknameUser(*(cmd.user), cmd.msg.getArgs()); //server function to register nickname change
	if (oldNick.size())
		sendMessage(":" + oldNick + " NICK " + cmd.user->getNickname() + "\r\n", cmd.user->getFd());
	// std::cout << "NICK command executed on " << cmd.msg.getArgs() << std::endl;
}
