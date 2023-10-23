#pragma once

#include "includes.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>

class Global {
	private:
		std::vector<Server *> servers;

	public:
		void	addServer(Server *server);
		// void	openMimeFile(std::string Mimestr);
		// void	FillMimeType(std::string line);

		void  print();

		Global();
		~Global();
		Global(const Global &copy);
		Global &operator=(const Global &copy);
};
