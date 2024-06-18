NAME = ircserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

SRC =	srcs/main.cpp srcs/Server.cpp srcs/Channel.cpp srcs/Message.cpp srcs/Commands.cpp srcs/User.cpp srcs/Commands/invite.cpp \
		srcs/Commands/join.cpp srcs/Commands/kick.cpp srcs/Commands/mode.cpp srcs/Commands/nick.cpp srcs/Commands/topic.cpp srcs/Commands/privmsg.cpp \
		srcs/Commands/user.cpp srcs/Commands/pass.cpp srcs/Commands/ping.cpp srcs/Commands/whois.cpp srcs/Commands/part.cpp srcs/Commands/quit.cpp 

HEADER = srcs/Server.hpp srcs/Channel.hpp srcs/Message.hpp srcs/Commands.hpp srcs/User.hpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

nclean: all clean

.PHONY: all re clean fclean nclean
