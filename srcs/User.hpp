#ifndef USER_HPP
# define USER_HPP

# include <unistd.h>
# include <sys/types.h>
# include <iostream>
# include <string>
# include <map>

typedef std::map<std::string, bool> JoinedChanMap;
typedef std::map<std::string, bool> InvitedChanMap;

class User {

    public :

		const std::string&		getNickname() const;
		void					setNickname(std::string nickname);

		const std::string&		getUsername() const;
		void					setUsername(std::string username);

		const std::string&		getRealname() const;
		void					setRealname(std::string realname);

		const std::string&		getHostname() const;
		void					setHostname(std::string realname);

		const std::string&		getServername() const;
		void					setServername(std::string realname);

		const int&				getFd() const;
		void					setFd(int fd);

		const JoinedChanMap&	getJoinedchannels() const;
		void					addJoinedChannel(std::string channel, bool op);
		void					delJoinedChannel(std::string channel);

		const InvitedChanMap&	getInvitedchannels() const;
		void					addInvitedChannel(std::string channel);
		void					delInvitedChannel(std::string channel);

		bool					isRegistered() const;
		void					setRegistered(bool val);
		void					finishRegistration();

		void					setPassed(bool val);
		const bool				&getPassed() const;

		void					addBuf(std::string buf);
		void					delBuf();
		std::string				getBuf();

       	User();
		~User();


    private :

        std::string		_nickname;
        std::string		_username;
        std::string		_realname;
		std::string 	_hostname;
		std::string 	_servername;
        int 			_fd;
		JoinedChanMap	_joinedchannels;
		InvitedChanMap	_invitedchannels;
		bool			_registered;
		bool			_passed;
		std::string		_userbuf;
};

#endif