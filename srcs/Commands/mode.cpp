#include "../Commands.hpp"

typedef	struct		s_mode {
	std::string					channel;
	std::vector<std::string>	modes;
	std::vector<std::string>	params;

	s_mode &operator=(s_mode const &value)
	{
		this->channel = value.channel;
		// std::cout << this->channel << std::endl;
		// std::cout << value.modes.size() << std::endl;
		if (value.modes.size())
			this->modes = value.modes;
		if (value.params.size())
			this->params = value.params;
		return (*this);
	}
}					t_mode;



t_mode	*parseModeArgs(std::string args_in) { //separe les arguments
	t_mode		*ret;
	std::string	arr;
	size_t		pos, pos2;
	std::string	args;

	args = args_in;
	ret = new t_mode;
	pos = args.find(' ');
	// std::cout << "ARGS_IN :" << args_in << std::endl;
	// std::cout << "POS :" << pos << std::endl;
	// std::cout << "RET :" << ret->modes.size() << std::endl;
	if (pos == std::string::npos) {
		if (args[0] != '#' && args[0] != '&' && args[0] != '+' && args[0] != '!')
			return (ret);
		ret->channel = args;
		ret->params.push_back("");
		return (ret);
	}
	arr = args.substr(0, pos);
	if (arr[0] != '#' && arr[0] != '&' && arr[0] != '+' && arr[0] != '!')
			return (ret);
	ret->channel = arr;
	args.erase(0, pos + 1);
	pos = args.find(' ');
	// std::cout << "POS :" << pos << std::endl;
	if (pos == std::string::npos && args.size() > 0) {
		// std::cout << "ARGS : " << args << std::endl;
		arr = args.substr(0, args.size());
		// std::cout << "ARR : " << arr << std::endl;
		ret->modes.push_back(arr);
		ret->params.push_back("");
		return (ret);
	}
	while (pos != std::string::npos) {
		arr = args.substr(0, pos);
		// std::cout << "ARR : " << arr << std::endl;
		ret->modes.push_back(arr);
		// std::cout << "PUSH_BACK : " << *(ret->modes.end() - 1) << std::endl;
		args.erase(0, pos + 1);
		// std::cout << "ARGS : " << args << std::endl;
		pos2 = args.find(' ');
		if (pos2 == std::string::npos && args.empty())
			return (ret);
		arr = args.substr(0, pos2);
		// std::cout << "ARR : " << arr << std::endl;
		ret->params.push_back(arr);
		args.erase(0, pos2 + 1);
		// std::cout << "ARGS : " << args << std::endl;
		pos = args.find(' ');
	}
	// std::cout << "RET " << ret->modes.size() << " " << ret->params.size() << std::endl;
	return (ret);
};

void	mode(t_cmd cmd) {
	ChannelMap::const_iterator	it;
	Channel						channel;
	t_mode						&parsed_args = *parseModeArgs(cmd.msg.getArgs());
	std::string					ChanName;
	bool						sign;

	// std::cout << "MODE ARGS : " << cmd.msg.getArgs() << " " << parsed_args.modes.size() << " " << parsed_args.channel.size() << std::endl;
	if (parsed_args.modes.size() < 1 || parsed_args.channel.size() < 1) {
		sendErrorMessage("461", cmd.msg.getCommand() + " :command requires more parameters.", cmd.user->getNickname(), cmd.user->getFd());
		delete &parsed_args;
		return ;
	}
	ChanName = parsed_args.channel;
	// std::cout << "CHAN NAME : " << ChanName << std::endl;
	it = cmd.serv->getChannels().find(ChanName);
	if (it == cmd.serv->getChannels().end()) {
		sendErrorMessage("403", ChanName + " :given channel name is invalid", cmd.user->getNickname(), cmd.user->getFd());
		delete &parsed_args;
		return ;
	}
	if (!cmd.user->getJoinedchannels().find(ChanName)->second) {
		sendErrorMessage("482", ChanName + " :not enough channel privileges.", cmd.user->getNickname(), cmd.user->getFd());
		delete &parsed_args;
		return ;
	}
	channel = cmd.serv->getChannels().find(ChanName)->second;
	for (size_t i = 0; i < parsed_args.modes.size(); i++) {
		if (parsed_args.modes[i].at(0) == '-')
			sign = false;
		else if (parsed_args.modes[i].at(0) == '+')
			sign = true;
		else {
			sendErrorMessage("472", parsed_args.modes[i].substr(0, 1) + " :'+' or '-' needed for mode instruction.", cmd.user->getNickname(), cmd.user->getFd());
			delete &parsed_args;
			return ;
		}
		for (size_t j = 0; j < parsed_args.modes[i].size(); j++) {
			//std::cout << "MODE : " << parsed_args.modes[i].at(j) << std::endl;
			if (parsed_args.modes[i].at(j) == 'i') {
				cmd.serv->setInviteChannel(ChanName, sign);
				sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com MODE " + channel.getName() + " " + parsed_args.modes[i] + "\r\n", cmd.user->getFd());
			}
			else if (parsed_args.modes[i].at(j) == 't') {
				cmd.serv->setTopicOpChannel(ChanName, sign);
				sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com MODE " + channel.getName() + " " + parsed_args.modes[i] + "\r\n", cmd.user->getFd());
			}
			else if (parsed_args.modes[i].at(j) == 'k') {
				if (sign) {
					if (parsed_args.params[i].empty()) {
						sendErrorMessage("461", cmd.msg.getCommand() + " :command requires more parameters.", cmd.user->getNickname(), cmd.user->getFd());
						delete &parsed_args;
						return ;
					}
					cmd.serv->setKeyChannel(ChanName, parsed_args.params[i]);
					//std::cout << "KEY : " << cmd.serv->getChannels().find(ChanName)->second.getKey() << std::endl;
				}
				else
					cmd.serv->removeKeyChannel(ChanName);
				sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com MODE " + channel.getName() + " " + parsed_args.modes[i] + "\r\n", cmd.user->getFd());
			}
			else if (parsed_args.modes[i].at(j) == 'o') {
				// std::cout << "PARAMS : " << parsed_args.params[i] << std::endl;
				if (parsed_args.params[i].empty()) {
						sendErrorMessage("461", cmd.msg.getCommand() + " :command requires more parameters.", cmd.user->getNickname(), cmd.user->getFd());
						delete &parsed_args;
						return ;
				}
				cmd.serv->getUsersFd().find(cmd.serv->getUserFd(parsed_args.params[i]))->second.delJoinedChannel(ChanName);
				if (sign)
					cmd.serv->getUsersFd().find(cmd.serv->getUserFd(parsed_args.params[i]))->second.addJoinedChannel(ChanName, true);
				else
					cmd.serv->getUsersFd().find(cmd.serv->getUserFd(parsed_args.params[i]))->second.addJoinedChannel(ChanName, false);
				sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com MODE " + channel.getName() + " " + parsed_args.modes[i] + "\r\n", cmd.user->getFd());
			}
			else if (parsed_args.modes[i].at(j) == 'l') {
				if (sign) {
					if (parsed_args.params[i].empty()) {
						sendErrorMessage("461", cmd.msg.getCommand() + " :command requires more parameters.", cmd.user->getNickname(), cmd.user->getFd());
						delete &parsed_args;
						return ;
					}
					cmd.serv->setLimitChannel(ChanName, atoi(parsed_args.params[i].c_str()));
				}
				else
					cmd.serv->removeLimitChannel(ChanName);
				sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com MODE " + channel.getName() + " " + parsed_args.modes[i] + "\r\n", cmd.user->getFd());
			}
		}
	}

	delete &parsed_args;
	// std::cout << "MODE command executed on " << cmd.msg.getArgs() << std::endl;
};
