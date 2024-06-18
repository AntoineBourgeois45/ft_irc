#include "../Commands.hpp"

typedef	struct		s_part {
	std::vector<std::string>	channels;
	std::string					message;
}					t_part;

t_part	*parsePartArgs(std::string args) { //separe les arguments
	t_part		*ret = new t_part;
	std::string	arr;
	size_t		pos, pos2;
	std::string	channels;

	pos = args.find(' ');
	if (pos == std::string::npos) {
		arr = args;
		channels = arr;
		pos2 = channels.find(',');
		if (pos2 == std::string::npos) {
			arr = channels.substr(0, pos2);
			ret->channels.push_back(arr);
			channels.erase(0, pos2 + 1);
		}
		while ((pos2 = channels.find(',')) != std::string::npos) {
			arr = channels.substr(0, pos2);
			ret->channels.push_back(arr);
			channels.erase(0, pos2 + 1);
		}
		arr = channels;
		return (ret);
	}
	arr = args.substr(0, pos);
	channels = arr;
	pos2 = channels.find(',');
	if (pos2 == std::string::npos) {
		arr = channels.substr(0, pos2);
		ret->channels.push_back(arr);
		channels.erase(0, pos2 + 1);
	}
	while ((pos2 = channels.find(',')) != std::string::npos) {
		arr = channels.substr(0, pos2);
		ret->channels.push_back(arr);
		channels.erase(0, pos2 + 1);
	}
	ret->message = args.substr(pos + 1, args.size());
	return (ret);
};

void	part(t_cmd cmd) {
	ChannelMap::const_iterator	it;
	Channel						channel;
	t_part						&parsed_args = *parsePartArgs(cmd.msg.getArgs());
	std::string					ChanName;

	if (parsed_args.channels.size() < 1) {
		sendErrorMessage("461", cmd.msg.getCommand() + " :command requires more parameters.", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	for (size_t i = 0; i < parsed_args.channels.size(); i++) {
		ChanName = parsed_args.channels[i];
		it = cmd.serv->getChannels().find(ChanName);
		if (it == cmd.serv->getChannels().end()) {
			sendErrorMessage("403", ChanName + " :no such channel.", cmd.user->getNickname(), cmd.user->getFd());
			return ;
		}
		if (it->second.getUserMap().find(cmd.serv->getUserFd(cmd.user->getNickname())) == it->second.getUserMap().end()) {
			sendErrorMessage("442", ChanName + " :you are not on the channel.", cmd.user->getNickname(), cmd.user->getFd());
			return ;
		}
		cmd.serv->removeUserFromChannel(cmd.user->getFd(), ChanName);
		channel = cmd.serv->getChannels().find(ChanName)->second;
		cmd.user->delJoinedChannel(ChanName);
		cmd.user->delInvitedChannel(ChanName);
		sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com PART " + channel.getName() + " " + parsed_args.message + "\r\n", cmd.user->getFd());
		if (channel.getUserNumber() > 0)
			sendGlobalMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com PART " + channel.getName() + " " + parsed_args.message + "\r\n", channel, cmd.user->getFd());
		if (!cmd.serv->getChannels().find(ChanName)->second.getUserNumber())
			cmd.serv->removeChannel(ChanName);
		// std::cout << "PART for client fd : \"" << cmd.user->getFd() << "\"" << std::endl;
		// std::cout << "User number : \"" << channel.getUserNumber() << "\"" << std::endl;
	}
	delete &parsed_args;
	// std::cout << "PART command executed." << std::endl;
};