#include "../Commands.hpp"

static std::vector<std::string>	parseArgs(std::string args)
{
	std::vector<std::string>	ret;
	size_t						pos;

	pos = args.find(" :");
	ret.insert(ret.end(), args.substr(0, pos));
	if (pos != std::string::npos)
		ret.insert(ret.end(), args.substr(pos + 2));
	return (ret);
}

void	topic(t_cmd cmd) {
	std::vector<std::string> args;

	args = parseArgs(cmd.msg.getArgs());
	if (cmd.serv->getChannels().find(args[0]) == cmd.serv->getChannels().end()) {
		sendErrorMessage("403", args[0] + " :channel does not exists", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	else {
		//must be on the channel to change or view its topic
		if (cmd.user->getJoinedchannels().find(args[0]) == cmd.user->getJoinedchannels().end())
		{
			sendErrorMessage("442", args[0] + " :you are not on that channel", cmd.user->getNickname(), cmd.user->getFd());
			return ;
		}
		//if channel topic is operator only and user is not operator
		else if (cmd.serv->getChannels().find(args[0])->second.getTopicOp() == true && !cmd.user->getJoinedchannels().find(args[0])->second)
		{
			sendErrorMessage("482", args[0] + " :you are not channel operator", cmd.user->getNickname(), cmd.user->getFd());
			return ;
		}
		//means a topic change
		if (args.size() == 2)
		{
			cmd.serv->changeChannelTopic(args[0], args[1]);
			sendGlobalErrorMessage("332", cmd.serv->getChannels().find(args[0])->second.getName() + " :" + cmd.serv->getChannels().find(args[0])->second.getTopic(), cmd.serv->getChannels().find(args[0])->second);
		}
		//returns topic
		else if (cmd.serv->getChannels().find(args[0])->second.getTopic().size()) {
			{
				sendErrorMessage("332", cmd.serv->getChannels().find(args[0])->second.getName() + " :" + cmd.serv->getChannels().find(args[0])->second.getTopic(),cmd.user->getNickname(), cmd.user->getFd());
				return ;
			}
		}
		else {
			sendErrorMessage("331", cmd.serv->getChannels().find(args[0])->second.getName() + ": " + "No topic set", cmd.user->getNickname(), cmd.user->getFd());
			return ;
		}
	}
	// std::cout << "TOPIC command executed on " << cmd.msg.getArgs() << std::endl;
};
