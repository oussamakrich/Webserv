#pragma once

#include "Server.hpp"
#include "Global.hpp"
#include "includes.hpp"
#include "GenerateLocation.hpp"


#define HOST_PORT std::pair<int, std::string>


class GenerateGlobalClass {

	public:
		static Global *generateGlobalClass(std::vector<TOKEN> tokens);
		static Server *fillServer(TOKEN_IT &it); 
		static bool			checkHostAndPort(Server *server, std::vector<HOST_PORT> &vect);
};
