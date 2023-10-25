#pragma once

#include "includes.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>
#include <sys/poll.h>

class Global {
	private:
		std::vector<Server *> servers;
		static	std::vector<struct pollfd> gPollFds; 

		
	public:
		void	addServer(Server *server);
		void	run();
		static void	insertFd(int fd);
		void	callHandelFds(struct pollfd pfd);
		
		
		void  print();

		Global();
		~Global();
		Global(const Global &copy);

		Global &operator=(const Global &copy);
};
