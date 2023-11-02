#pragma once

#include "../../include/includes.hpp"
#include "Server.hpp"
#include "Global.hpp"

class Global {
	private:
		std::vector<Server *> servers;
		static	std::vector<struct pollfd> gPollFds;


	public:
		void	addServer(Server *server);
		void	run();
		static void	insertFd(int fd);
		static void removeFd(int fd);
		static void switchEvent(int fd, int Flag);
		void	callHandelFds(struct pollfd pfd);



		void  print();

		Global();
		~Global();
		Global(const Global &copy);

		Global &operator=(const Global &copy);
};
