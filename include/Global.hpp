#pragma once

#include "includes.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>

class Global {
	private:
		std::vector<Server *> servers;
		static	std::vector<struct pollfd> gPollFds; 

		// std::vector<Clients *> clients;
		
	public:
		void	addServer(Server *server);
		void	run();
		static void	insertFd(int fd);
		
		
		void  print();

		Global();
		~Global();
		Global(const Global &copy);

		Global &operator=(const Global &copy);
};
