#pragma once

#include "includes.hpp"


class ParseConfig{

	private:
		ParseConfig();
		ParseConfig &operator=(ParseConfig const &copy);
		ParseConfig(ParseConfig const &copy);
		void FillMimeTypes(std::ifstream &file); // throw (std::exception);
		void SyntaxError(); // throw (std::exception);
		TOKEN_PAIR skipSpaces(std::vector<TOKEN_PAIR>::iterator &it);
		void checkLimit(std::vector<TOKEN_PAIR>::iterator &it);
		void checkToken(std::vector<TOKEN_PAIR>::iterator &it, token type);
		void closed();
		void FillServers(std::string &fileString); // throw (std::exception);


	private:
		std::map<std::string , std::string> mimeTypes;
		std::vector<TOKEN_PAIR> tokens;

	public:
		ParseConfig(std::string &path);
		~ParseConfig();
};

