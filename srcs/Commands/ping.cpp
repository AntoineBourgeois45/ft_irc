#include "../Commands.hpp"

void	ping(t_cmd cmd) //replies pong to client to let it know it's still connected
{
	if (cmd.msg.getArgs() == cmd.serv->getServerName())
	{
		sendMessage("PONG " + cmd.msg.getArgs() + "\r\n", cmd.user->getFd());
	}
}