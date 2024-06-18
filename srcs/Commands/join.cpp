#include "../Commands.hpp"
#include <cctype>
#include <iostream>

typedef	struct		s_arg {
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;
}					t_arg;

t_arg	*parseJoinArgs(std::string args) { //separe les arguments
	t_arg		*ret = new t_arg;
	std::string	arr;
	size_t		pos, pos2;
	std::string	channels, keys;

	pos = args.find(' ');
	if (pos == std::string::npos) {
		arr = args;
		channels = arr;
		while ((pos2 = channels.find(',')) != std::string::npos) {
			arr = channels.substr(0, pos2);
			ret->channels.push_back(arr);
			channels.erase(0, pos2 + 1);
		}
		if (pos2 == std::string::npos) {
			arr = channels.substr(0, pos2);
			ret->channels.push_back(arr);
			channels.erase(0, pos2 + 1);
		}
		arr = channels;
		return (ret);
	}
	channels = args.substr(0, pos);
	keys = args.substr(pos + 1);
	pos2 = channels.find(',');
	if (pos2 == std::string::npos) {
		arr = channels.substr(0, pos2);
		ret->channels.push_back(arr);
	}
	while (pos2 != std::string::npos) {
		arr = channels.substr(0, pos2);
		ret->channels.push_back(arr);
		channels.erase(0, pos2 + 1);
		pos2 = channels.find(',');
	}
	pos2 = keys.find(',');
	if (pos2 == std::string::npos) {
		arr = keys.substr(0, pos2);
		ret->keys.push_back(arr);
	}
	while (pos2 != std::string::npos) {
		arr = keys.substr(0, pos2);
		ret->keys.push_back(arr);
		keys.erase(0, pos2 + 1);
		pos2 = keys.find(',');
	}
	// std::cout << "CHANNEL : \"" << ret->channels[0] << "\"" << std::endl;
	// std::cout << "KEY : \"" << ret->keys[0] << "\"" << std::endl;
	return (ret);
};

int	parseChanName(std::string args) { //check si le nom du channel respecte les regles d'IRC
	size_t 		pos;

	pos = args.find('#');
	if (pos == std::string::npos || pos > 0) {
		pos = args.find('&');
		if (pos == std::string::npos || pos > 0) {
			pos = args.find('+');
			if (pos == std::string::npos || pos > 0) {
				pos = args.find('!');
				if (pos == std::string::npos || pos > 0) {
					return (-1);
				}
			}
		}
	}
	if (args.size() > 50)
		return (-1);
	pos = args.find(',');
	if (pos != std::string::npos)
		return (-1);
	pos = args.find(':');
	if (pos != std::string::npos)
		return (-1);
	pos = args.find(7);
	if (pos != std::string::npos)
		return (-1);
	for (int i = 0; args[i]; i++) {
		if (isalpha(args[i]))
			args[i] = tolower(args[i]);
	}
	return (0);
};

void	join(t_cmd cmd) {
	ChannelMap::const_iterator	it;
	Channel						channel;
	t_arg						&parsed_args = *parseJoinArgs(cmd.msg.getArgs());
	std::string					ChanName, ChanKey;
	bool						op;

	if (parsed_args.channels.size() < 1) {
		sendErrorMessage("461", cmd.msg.getCommand() + " :command requires more parameters.", cmd.user->getNickname(), cmd.user->getFd());
		delete &parsed_args;
		return ;
	}
	for (size_t i = 0; i < parsed_args.channels.size(); i++) {
		ChanName = parsed_args.channels[i];
		if (parseChanName(ChanName) == -1) {
			sendErrorMessage("403", ChanName + " :given channel name is invalid.", cmd.user->getNickname(), cmd.user->getFd());
			delete &parsed_args;
			return ;
		}
		it = cmd.serv->getChannels().find(ChanName);
		op = false;
		if (it == cmd.serv->getChannels().end()) {
			cmd.serv->addChannel(ChanName);
			cmd.user->addJoinedChannel(ChanName, true);
			op = true;
		}
		channel = cmd.serv->getChannels().find(ChanName)->second;
		if (channel.getLimit() == true && channel.getUserNumber() >= channel.getLimitNum()) {
			sendErrorMessage("471", ChanName + " :channel is full.", cmd.user->getNickname(), cmd.user->getFd());
			delete &parsed_args;
			return ;
		}
		if (channel.getInvite() == true && cmd.serv->getUsersFd().find(cmd.serv->getUserFd(cmd.user->getNickname()))->second.getInvitedchannels().find(ChanName) == cmd.serv->getUsersFd().find(cmd.serv->getUserFd(cmd.user->getNickname()))->second.getInvitedchannels().end()) { //add condition
			sendErrorMessage("473", ChanName + " :channel is invite only.", cmd.user->getNickname(), cmd.user->getFd());
			delete &parsed_args;
			return ;
		}
		if (channel.getIsKey() == true) {
			if (i < parsed_args.keys.size()) {
				ChanKey = parsed_args.keys[i];
				if (channel.getKey() != ChanKey) {
					sendErrorMessage("475", ChanName + " :channel is key-locked, key is either missing or wrong.", cmd.user->getNickname(), cmd.user->getFd());
					delete &parsed_args;
					return ;
				}
			}
			else {
				sendErrorMessage("475", ChanName + " :channel is key-locked, key is either missing or wrong.", cmd.user->getNickname(), cmd.user->getFd());
				delete &parsed_args;
				return ;
			}
		}
		cmd.serv->addUsertoChannel(cmd.user, ChanName);
		if (op == false)
			cmd.user->addJoinedChannel(ChanName, false);
		else
			sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com MODE " + channel.getName() + " +o\r\n", cmd.user->getFd());
		sendMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com JOIN " + channel.getName() + "\r\n", cmd.user->getFd());
		if (cmd.serv->getChannels().find(ChanName)->second.getUserNumber() > 0)
			sendGlobalMessage(":" + cmd.user->getNickname() + "!" + cmd.user->getUsername() + "@test.server.com JOIN " + channel.getName() + "\r\n", channel, cmd.user->getFd());
		sendErrorMessage("332", cmd.serv->getChannels().find(ChanName)->second.getName() + " :" + cmd.serv->getChannels().find(ChanName)->second.getTopic(),cmd.user->getNickname(), cmd.user->getFd());
	}
	delete &parsed_args;
	// std::cout << "JOIN command executed." << std::endl;
};