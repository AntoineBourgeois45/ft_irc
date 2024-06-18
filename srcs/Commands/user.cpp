#include "../Commands.hpp"

static std::vector<std::string>	parseArgs(std::string args)
{
	std::vector<std::string>	ret;
	size_t						pos;
	size_t						pos2;

	pos = 0;
	pos2 = 0;
	pos2 = args.find(" ", pos);
	while (pos2 != std::string::npos && pos2 < args.find(":") && ret.size() < 3)
	{
		if (pos2 != std::string::npos)
		{
			ret.insert(ret.end(), args.substr(pos, pos2 - pos));
			pos = pos2 + 1;
		}
		pos2 = args.find(" ", pos);
	}
	if (args.find(":") != args.npos)
		ret.insert(ret.end(), args.substr(args.find(":")));
	return (ret);
}

void	user(t_cmd cmd)
{
	std::vector<std::string> args;

	if (cmd.serv->getUsersFd().find(cmd.user->getFd())->second.getUsername().size())
	{
		sendErrorMessage("462", ":Already registered", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	args = parseArgs(cmd.msg.getArgs());
	if (args.size() < 4)
	{
		sendErrorMessage("461", "USER :Error with parameters", cmd.user->getNickname(), cmd.user->getFd());
		return ;
	}
	cmd.user->setUsername(args[0]);
	cmd.user->setHostname("localhost"); //hard coded because the socket is bound to 127.0.0.1
	cmd.user->setServername(args[2]);
	cmd.user->setRealname(args[3]);
}