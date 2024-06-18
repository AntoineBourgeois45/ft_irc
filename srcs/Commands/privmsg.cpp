#include "../Commands.hpp"
#include "../Message.hpp"


static std::vector<std::string>	parseArgs(std::string args)
{
	std::vector<std::string>	ret;
	size_t						pos;
	size_t						pos2;

	pos = 0;
	pos2 = args.find(",", pos);
	while (pos2 != std::string::npos && pos2 < args.find(" ", pos))
	{
		pos2 = args.find(",", pos);
		if (pos2 != std::string::npos)
		{
			ret.insert(ret.end(), args.substr(pos, pos2 - pos));
			pos = pos2 + 1;
		}
	}
	pos2 = args.find(" ", pos);
	if (pos2 != std::string::npos)
	{
		ret.insert(ret.end(), args.substr(pos, pos2 - pos));
		ret.insert(ret.end(), args.substr(pos2 + 1));
	}
	else
	{
		ret.push_back("");
	}
	return (ret);
}

void	privmsg(t_cmd cmd) {
	std::vector<std::string> args;
	std::vector<std::string>::iterator itb;
	std::vector<std::string>::iterator ite;

	args = parseArgs(cmd.msg.getArgs());
	itb = args.begin();
	ite = args.end();
	if ((ite - 1)->size() == 0) {
		sendErrorMessage("412", "Not enough arguments", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	while (itb != ite - 1) {
		if (itb->at(0) == '#' || itb->at(0) == '!' || itb->at(0) == '&' || itb->at(0) == '+')
		{
			if (cmd.serv->getChannels().find(*itb) == cmd.serv->getChannels().end()) {
				sendErrorMessage("403", *itb + ": No such channel", cmd.user->getNickname(), cmd.user->getFd());
			}
			else //sends message to everyone on a channel exept its sender
				sendGlobalMessage(":" + cmd.user->getNickname() + " PRIVMSG " + *itb + " " + *(ite - 1) + "\r\n", (cmd.serv->getChannels().find(*itb))->second, cmd.user->getFd());
		}
		else {
			if (cmd.serv->getUsersNick().find(*itb) == cmd.serv->getUsersNick().end()) {
				sendErrorMessage("401", *itb + " No such Nick", cmd.user->getNickname(), cmd.user->getFd());
			}
			else
			{
				sendMessage(":" + cmd.user->getNickname() + " PRIVMSG " + *itb + " " + *(ite - 1) + "\r\n", cmd.serv->getUsersNick().find(*itb)->second);
			}
		}
		itb++;
	}
	// std::cout << "PRIVMSG command executed on " << cmd.msg.getArgs() << std::endl;
};
