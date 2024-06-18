/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anbourge <anbourge@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 17:55:33 by anbourge          #+#    #+#             */
/*   Updated: 2023/12/03 17:02:08 by anbourge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <cstdlib>
#include <csignal>

#include "Server.hpp"

bool	work = true;

void	sigHandler(int signum) {
	(void)signum;
	work = false;
}

int	main(int argc, char **argv) {
	Server	s;
	
	if (argc != 3) {
		std::cerr << "Wrong number of arguments\n";
		return 0;
	}
	s.addPort(argv[1]);
	s.addPw(argv[2]);
	s.setServerName("test.server.com");
	s.createSocket();
	s.listenSocket();
	s.createEpoll();
	while (work) {
		signal(SIGINT, &sigHandler);
		s.serverConnection();
	}
}