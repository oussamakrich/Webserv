#pragma once

#include "includes.hpp"
#include "GenerateServers.hpp"
#include "Server.hpp"

class ParseConfig{

	private:
		ParseConfig();
		ParseConfig &operator=(ParseConfig const &copy);
		ParseConfig(ParseConfig const &copy);
		void FillMimeTypes(); // throw (std::exception);
		void SyntaxError(); // throw (std::exception);
		TOKEN_PAIR skipSpaces(std::vector<TOKEN_PAIR>::iterator &it);
		void checkToken(std::vector<TOKEN_PAIR>::iterator &it, token type);
		void closed();
		void FillServers(std::string &fileString); // throw (std::exception);
		void getMimeFile(std::vector<TOKEN_PAIR>::iterator it);
		void checkBeforToken(std::vector<TOKEN_PAIR>::iterator it);


	private:
		std::string mimeTypeFile;
		std::map<std::string , std::string> mimeTypes;
		std::vector<TOKEN_PAIR> tokens;
		std::vector<Server*> servers;
		std::string key;
		std::string value;
		std::stringstream ss;
		bool newMime;
		GenerateServers generator;
	public:
		ParseConfig(std::string &path);
		~ParseConfig();
};

