#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <unistd.h>
# include <sys/types.h>
# include <netdb.h>
# include <iostream>
# include <string>
# include <map>
# include "User.hpp"

# define NB_FLAGS 1

typedef std::map<int, User*> UserpointMapFd;

class Channel {

    public :
		Channel();
		Channel(std::string name);
		~Channel();

	    const std::string	&getName() const;
		void				setName(std::string name);

		const int			&getLimitNum() const;
		const bool			&getLimit() const;
		void				setLimit(int limit);
		void				removeLimit();

		const bool			&getInvite() const;
		void				setInvite(bool inv);

		const std::string	&getKey() const;
		const bool			&getIsKey() const;
		void				setKey(std::string key);
		void				removeKey();

		const std::string	&getTopic() const;
		void				setTopic(std::string topic);

		const bool			&getTopicOp() const;
		void				setTopicOp(bool op);

		const UserpointMapFd	&getUserMap() const;
		const int			&getUserNumber() const;

		void				addUser(User *user);
		void				kickUser(User *user);

    private :

    	std::string 	_name;
		int				_ulimit;
		bool			_islimit;
		int				_unumber;
		bool			_inviteonly;
		bool			_iskey;
		std::string		_key;
		std::string		_topic;
		bool			_topicop;
		UserpointMapFd	_users;
};

#endif