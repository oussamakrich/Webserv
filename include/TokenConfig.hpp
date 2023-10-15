
#pragma once

#include "includes.hpp"

class TokenConfig {


public:
	TokenConfig();
	TokenConfig(const TokenConfig &copy);
	TokenConfig &operator=(const TokenConfig &copy);
	~TokenConfig();

	std::vector<TOKEN_PAIR> TokenTheConfig(std::ifstream &file);

private:
	void tokenLine(std::string &line);
	void tokenWord(std::string &line);

private:
	std::vector<TOKEN_PAIR> tokens;
};

