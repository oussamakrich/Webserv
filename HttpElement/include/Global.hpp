#pragma once

#include "../../include/includes.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include <vector>


#define MAP_STRING std::map<std::string, std::string>
#define MAP_STRITT std::map<std::string, std::string>::iterator

class Global {
	private:
		static	std::vector<struct pollfd> gPollFds;


	public:
		void	addServer(Server *server);
		void	run();
		static void	insertFd(int fd);
		static void removeFd(int fd);
		static void switchEvent(int fd, int Flag);
		void	callHandelFds(struct pollfd pfd);

		static  std::vector<std::string> serverNames;
	 static Server &FindServer( const MAP_STRING  &headers, Server &ser);
		static std::vector<Server *> servers;



		void  print();

		Global();
		~Global();
		Global(const Global &copy);

		Global &operator=(const Global &copy);
};
