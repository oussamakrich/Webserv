#pragma once

#include "includes.hpp"
#include "Server.hpp"

class Global {
	private:
		std::map<std::string, std::string> mime_types;
		std::vector<Server *> servers;

	public:
		void addServer(Server *server);
		Global();
		~Global();
		Global(const Global &copy);
		Global &operator=(const Global &copy);
};
