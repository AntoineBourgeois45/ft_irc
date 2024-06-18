#include "../Commands.hpp"

void	pass(t_cmd cmd) //password command, user is disconnected if it returns 1
{
	std::string	msg;
	Channel	channel;
	int		fd;
	
	// std::cout << "PASSWORD : " << cmd.msg.getArgs() << std::endl;
	if (cmd.serv->getPw() != cmd.msg.getArgs()) {
		msg.assign(cmd.serv->getPw() + " :irc.example.org 464 * :Password incorrect\r\n");
		sendMessage(msg, cmd.user->getFd());
		fd = cmd.user->getFd();
		while (cmd.user->getJoinedchannels().size() > 0)
		{
			channel = cmd.serv->getChannels().find(cmd.user->getJoinedchannels().begin()->first)->second;
			cmd.serv->removeUserFromChannel(cmd.user->getFd(), channel.getName());
			cmd.user->delJoinedChannel(channel.getName());
			if (!cmd.serv->getChannels().find(channel.getName())->second.getUserNumber())
				cmd.serv->removeChannel(channel.getName());
		}
		cmd.serv->removeUser(cmd.user->getFd());
		close(fd);
			std::cerr << "Wrong password.\n";
	}
	else {
		cmd.user->setPassed(true);
	}
	// std::cout << "Good password\n";
}