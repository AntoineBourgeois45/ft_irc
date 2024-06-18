#include "../Commands.hpp"

typedef	 struct		s_args {
	std::vector<std::string>	channels;
	std::vector<std::string>	nicknames;
	std::string					reason;
}					t_args;

t_args	*parseKickArgs(std::string args) { //separe les arguments
	t_args		*ret = new t_args();
	std::string	arr;
	size_t		pos, pos2;
	std::string	channels, nicknames, reason;

	pos = args.find(' ');
	if (pos == std::string::npos)
		return (ret);
	channels = args.substr(0, pos);
	//std::cout << "KICK CHANNELS : " << channels << std::endl;
	args.erase(0, pos + 1);
	pos = args.find(' ');
	nicknames = args.substr(0, pos);
	//std::cout << "KICK NICKNAMES : " << nicknames << std::endl;
	args.erase(0, pos + 1);
	if (args.size() > 0) {
		reason = args.substr(0, args.size());
		ret->reason = reason.substr(0, reason.size());
	}
	while ((pos2 = channels.find(',')) != std::string::npos) {
		arr = channels.substr(0, pos2);
		ret->channels.push_back(arr);
		channels.erase(0, pos2 + 1);
	}
	arr = channels.substr(0, channels.size());
	ret->channels.push_back(arr);
	if (nicknames.size()) {
		while ((pos2 = nicknames.find(',')) != std::string::npos) {
			arr = nicknames.substr(0, pos2);
			ret->nicknames.push_back(arr);
			nicknames.erase(0, pos2 + 1);
		}
		arr = nicknames.substr(0, nicknames.size());
		ret->nicknames.push_back(arr);
	}
	//std::cout << "KICK REASON : " << ret->reason << " size : " << ret->reason.size() << std::endl;
	if (ret->reason.size() <= 1)
		ret->reason = "Get the hell outta here !";
	return (ret);
};

void	kick(t_cmd cmd) {
	ChannelMap::const_iterator	it;
	Channel						channel;
	t_args						*parsed_args = parseKickArgs(cmd.msg.getArgs());
	std::string					channame;

	// std::cout << "KICK ARGS : " << parsed_args->reason << std::endl;
	if (parsed_args->channels.size() < 1 || parsed_args->nicknames.size() < 1 || (parsed_args->channels.size() > 1 && (parsed_args->channels.size() != parsed_args->nicknames.size()))) {
		sendErrorMessage("461", cmd.msg.getCommand() + " :command requires more parameters.", cmd.user->getNickname(), cmd.user->getFd());
		delete parsed_args;
		return ;
	}
	for (size_t i = 0; i < parsed_args->channels.size(); i++) {
		channame = parsed_args->channels[i];
		it = cmd.serv->getChannels().find(channame);
		if (it == cmd.serv->getChannels().end()) {
			sendErrorMessage("403", channame + ":the given channel name is invalid, or does not exist.", cmd.user->getNickname(), cmd.user->getFd());
			delete parsed_args;
			return ;
		}
		if (it->second.getUserMap().find(cmd.serv->getUserFd(cmd.user->getNickname())) == it->second.getUserMap().end()) {
			sendErrorMessage("442", channame + " :you are not on the channel.", cmd.user->getNickname(), cmd.user->getFd());
			delete parsed_args;
			return ;
		}
		if (!cmd.user->getJoinedchannels().find(channame)->second) {
			sendErrorMessage("482", channame + " :not enough channel privileges.", cmd.user->getNickname(), cmd.user->getFd());
			delete parsed_args;
			return ;
		}
		//std::cout << "KICK NAME : \"" << parsed_args->nicknames[i] << "\"" << std::endl;
		if (it->second.getUserMap().find(cmd.serv->getUserFd(parsed_args->nicknames[i])) == it->second.getUserMap().end()) {
			sendErrorMessage("441", parsed_args->nicknames[i] + " " + channame + " :user is not on the channel.", cmd.user->getNickname(), cmd.user->getFd());
			delete parsed_args;
			return ;
		}
		channel = cmd.serv->getChannels().find(channame)->second;
		//std::cout << "KICK REASON : " << parsed_args->reason << std::endl;
		sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com KICK " + channel.getName() + " " + parsed_args->nicknames[i] + " " + parsed_args->reason + "\r\n", cmd.user->getFd());
		if (channel.getUserNumber() > 0)
			sendGlobalMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com KICK " + channel.getName() + " " + parsed_args->nicknames[i] + "\r\n", channel, cmd.user->getFd());
		cmd.serv->removeUserFromChannel(cmd.serv->getUserFd(parsed_args->nicknames[i]), channame);
		if (!cmd.serv->getChannels().find(channame)->second.getUserNumber())
			cmd.serv->removeChannel(channame);
	}
	delete parsed_args;
	// std::cout << "KICK command executed on " << cmd.msg.getArgs() << std::endl;
};
