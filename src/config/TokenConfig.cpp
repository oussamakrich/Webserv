#include "../../include/TokenConfig.hpp"


TokenConfig::TokenConfig(){
}

TokenConfig::~TokenConfig(){
}

TokenConfig &TokenConfig::operator=(const TokenConfig &copy){
	if (this != &copy)
	{
		this->tokens = copy.tokens;
	}
	return (*this);
}

TokenConfig::TokenConfig(const TokenConfig &copy){
	*this = copy;
}


void TokenConfig::tokenWord(std::string &line){
	size_t pos;
	std::string value;
	token				type;

	pos = line.find_first_of("\t;:[]{} ");
	value = line;
	if (pos != line.npos)
		value = line.substr(0, pos);
	type = VALUE;
	if (value == "server")
		type = SERVER;
	else if (value == "include")
		type = INCLUDE;
	else if (value == "location")
		type = LOCATION;
	this->tokens.push_back(std::make_pair(value, type));
	line.erase(0, pos);
}

//FIX : switch else if with switch case
void TokenConfig::tokenLine(std::string &line){

	token type;
	std::string search = " \t;:{}[]";
	while (!line.empty()){
		if (search.find(line[0]) != search.npos){
			if (line[0] == ' ' || line[0] == '\t') type = ESP;
			else if (line[0] == '[') type = BRACKETOPEN;
			else if (line[0] == ']') type = BRACKETCLOSE;
			else if (line[0] == ':') type = COLONE;
			else if (line[0] == ';') type =SEMICOLONE;
			else if( line[0] == '{') type = CURLYOPEN;
			else if( line[0] == '}') type = CURLYCLOSE;
			this->tokens.push_back(std::make_pair(line.substr(0, 1), type));
			line.erase(0, 1);
		}
		else
			tokenWord(line);
	}
}

std::vector<TOKEN_PAIR> TokenConfig::TokenTheConfig(std::ifstream &file){

	std::string line;
	std::string search = "{};[]";

	while (!file.eof())
	{
		std::getline(file, line, '\n');
		line = trim(line);
		if (line.empty())
			continue;
		if (search.find(line.back()) == search.npos)
			throw std::runtime_error("ERROR: line should be ended by special character");
		this->tokenLine(line);
	}
	this->tokens.push_back(std::make_pair("", END));
	return this->tokens;
}
