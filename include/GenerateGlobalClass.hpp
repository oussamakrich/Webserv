#pragma once

#include "Server.hpp"
#include "Global.hpp"
#include "includes.hpp"
#include "GenerateLocation.hpp"


class GenerateGlobalClass {
	private:

	public:
		static Global *generateGlobalClass(std::vector<TOKEN> &tokens);
		GenerateGlobalClass();
		~GenerateGlobalClass();
		GenerateGlobalClass(const GenerateGlobalClass &copy);
		GenerateGlobalClass &operator=(const GenerateGlobalClass &copy);
};
