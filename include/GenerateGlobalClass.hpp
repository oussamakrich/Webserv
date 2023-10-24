#pragma once

#include "Server.hpp"
#include "Global.hpp"
#include "includes.hpp"
#include "GenerateLocation.hpp"

class GenerateGlobalClass {

	public:
		static Global *generateGlobalClass(std::vector<TOKEN> tokens);
		static Server *fillServer(TOKEN_IT &it); 
};
