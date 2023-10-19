#pragma once

#include "Server.hpp"
#include "Global.hpp"

class GenerateGlobalClass {
	private:

	public:
		Global *generateGlobalClass(std::vector<Types, std::string> &tokens);
		GenerateGlobalClass();
		~GenerateGlobalClass();
		GenerateGlobalClass(const GenerateGlobalClass &copy);
		GenerateGlobalClass &operator=(const GenerateGlobalClass &copy);
};
