#pragma once

#include "includes.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>

class Global {
	private:
		std::map<std::string, std::string> mime_types;
		std::vector<Server *> servers;

	public:
		void	addServer(Server *server);
		void	HandelMimeTypes(TOKEN_IT &it);
		void	openMimeFile(std::string Mimestr);
		void	FillMimeType(std::string line);

		void  print();

		Global();
		~Global();
		Global(const Global &copy);
		Global &operator=(const Global &copy);
};
