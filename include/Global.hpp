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

		void  print(){
		std::vector<Server *>::iterator it = servers.begin();
		std::map<std::string, std::string>::iterator itt = mime_types.begin();

		std::cout <<"-------------MIME TYPES---------" << std::endl;
		for(;itt != mime_types.end(); itt++){
			std::cout <<  itt->first << "	:	" << itt->second<< std::endl;
		}

		for(;it != servers.end(); it++){
			Server *ser = *it;
			ser->print();
		}
	};

		Global();
		~Global();
		Global(const Global &copy);
		Global &operator=(const Global &copy);
};
