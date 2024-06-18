#include "../Commands.hpp"

std::vector<std::string>	parseArgsInv(std::string args) { //separe les arguments et check le nombre d'arguments
	std::vector<std::string>	ret;
	std::string					arr;
	size_t						pos;

	pos = args.find(' ');
	if (pos == std::string::npos) {
		arr = args.substr(0, args.size());
		ret.push_back(arr);
		return (ret);
	}
	arr = args.substr(0, pos);
	ret.push_back(arr);
	arr = args.substr(pos, args.size());
	ret.push_back(arr);
	return (ret);
};

void	invite(t_cmd cmd) {
	ChannelMap::const_iterator	it;
	std::string					nickname, channame, sender;
	std::vector<std::string>	parsed_args = parseArgsInv(cmd.msg.getArgs());
	
	if (parsed_args.size() < 2) {
		sendErrorMessage("461", cmd.msg.getCommand() + " :command requires more parameters.", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	nickname = parsed_args[0];
	channame = parsed_args[1];
	channame.erase(0, 1);
	sender = cmd.user->getNickname();
	if (cmd.serv->getUsersNick().find(nickname) == cmd.serv->getUsersNick().end()) {
		sendErrorMessage("401", nickname + " :no such user.", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	it = cmd.serv->getChannels().find(channame);
	if (it == cmd.serv->getChannels().end())
		return ; //the channel doesnt exist
	if (it->second.getUserMap().find(cmd.serv->getUserFd(nickname)) != it->second.getUserMap().end()) {
		sendErrorMessage("443", nickname + " " + channame + " :user is already on the channel.", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	if (it->second.getUserMap().find(cmd.serv->getUserFd(sender)) == it->second.getUserMap().end()) {
		sendErrorMessage("442", channame + " :you are not on the channel.", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	if (it->second.getInvite() && it->second.getUserMap().find(cmd.serv->getUserFd(cmd.user->getNickname()))->second->getJoinedchannels().find(channame)->second == false) {
		sendErrorMessage("482", channame + " :not enough channel privileges.", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	sendErrorMessage("341", nickname + " " + channame, cmd.user->getNickname(), cmd.user->getFd());
	sendMessage(nickname + " " + channame + "\r\n", cmd.user->getFd());
	sendMessage(":" + sender + "!" + cmd.user->getUsername() + "@" + cmd.serv->getServerName() + " INVITE " + nickname + " " + channame + "\r\n", cmd.serv->getUserFd(nickname));
	cmd.serv->getUsersFd().find(cmd.serv->getUserFd(nickname))->second.addInvitedChannel(channame);
	// std::cout << "INVITE command executed." << std::endl;
};
