#pragma once

#include "includes.hpp"


class ParseConfig{

	private:
		ParseConfig(){};
		ParseConfig &operator=(ParseConfig const &copy);
		ParseConfig(ParseConfig const &copy);
		void FillMimeTypes(std::ifstream &file); // throw (std::exception);
		void SyntaxError(std::string &fileString); // throw (std::exception);
		void FillServers(std::string &fileString); // throw (std::exception);
	

	private:
		std::map<std::string , std::string> mimeTypes;
		std::vector<TOKEN_PAIR> tokens;

	public:
		ParseConfig(std::string path);
		~ParseConfig();

};
