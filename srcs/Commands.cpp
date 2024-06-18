#include "Commands.hpp"

ptmf	getCommands(Message msg) {
	if (msg.getCommand().compare("JOIN") == 0) {
		void (*ptr)(t_cmd) = &join;
		return (ptr);
	}
	else if (msg.getCommand().compare("NICK") == 0) {
		void (*ptr)(t_cmd) = &nick;
		return (ptr);
	}
	else if (msg.getCommand().compare("KICK") == 0) {
		void (*ptr)(t_cmd) = &kick;
		return (ptr);
	}
	else if (msg.getCommand().compare("MODE") == 0) {
		void (*ptr)(t_cmd) = &mode;
		return (ptr);
	}
	else if (msg.getCommand().compare("INVITE") == 0) {
		void (*ptr)(t_cmd) = &invite;
		return (ptr);
	}
	else if (msg.getCommand().compare("TOPIC") == 0) {
		void (*ptr)(t_cmd) = &topic;
		return (ptr);
	}
	else if (msg.getCommand().compare("PRIVMSG") == 0) {
		void (*ptr)(t_cmd) = &privmsg;
		return (ptr);
	}
	else if (msg.getCommand().compare("USER") == 0) {
		void (*ptr)(t_cmd) = &user;
		return (ptr);
	}
	else if (msg.getCommand().compare("PING") == 0) {
		void (*ptr)(t_cmd) = &ping;
		return (ptr);
	}
	else if (msg.getCommand().compare("WHOIS") == 0) {
		void (*ptr)(t_cmd) = &whois;
		return (ptr);
	}
	else if (msg.getCommand().compare("QUIT") == 0) {
		void (*ptr)(t_cmd) = &quit;
		return (ptr);
	}
	else if (msg.getCommand().compare("PART") == 0) {
		void (*ptr)(t_cmd) = &part;
		return (ptr);
	}
	else if (msg.getCommand().compare("PASS") == 0) {
		void (*ptr)(t_cmd) = &pass;
		return (ptr);
	}
	else
		return (NULL);
};
