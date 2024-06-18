#include "../Commands.hpp"

void	quit(t_cmd cmd) {
	Channel	channel;
	int		fd;
	// std::cout << "QUIT command executed." << std::endl;

	fd = cmd.user->getFd();
	while (cmd.user->getJoinedchannels().size() > 0) {
		channel = cmd.serv->getChannels().find(cmd.user->getJoinedchannels().begin()->first)->second;
		cmd.serv->removeUserFromChannel(cmd.user->getFd(), channel.getName());
		cmd.user->delJoinedChannel(channel.getName());
		if (!cmd.serv->getChannels().find(channel.getName())->second.getUserNumber())
			cmd.serv->removeChannel(channel.getName());
	}
	sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@" + cmd.user->getHostname() + " QUIT " + cmd.msg.getArgs() + "\r\n", cmd.user->getFd());
	cmd.serv->removeUser(cmd.user->getFd());
	close(fd);
};