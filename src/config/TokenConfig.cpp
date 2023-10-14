#include "../../include/TokenConfig.hpp"


void TokenConfig::tokenWord(std::string &line){
	size_t pos;
	std::string value;
	token				type;

	pos = line.find_first_of(";:[] ");
	value = line;
	if (pos != line.npos)
		value = line.substr(0, pos);	
	type = VALUE;
	if (value == "server")
		type = SERVER;
	else if (value == "include")
		type = INCLUDE;
	this->tokens.push_back(std::make_pair(value, type));	
	line.erase(0, pos);
}


void TokenConfig::tokenLine(std::string &line){
	
	while (!line.empty()){
		if (line[0] == ' '){
			this->tokens.push_back(std::make_pair("", ESP));
			line.erase(0, 1);
		}
		else if (line[0] == '['){
			this->tokens.push_back(std::make_pair("",BRACKETOPEN));
			line.erase(0, 1);
		}
		else if (line[0] == ']'){
			this->tokens.push_back(std::make_pair("", BRACKETCLOSE));
			line.erase(0, 1);
		}
		else if (line[0] == ':'){
			this->tokens.push_back(std::make_pair("", COLONE));
			line.erase(0, 1);
		}
		else if (line[0] == ';'){
			this->tokens.push_back(std::make_pair("", SEMICOLONE));
			line.erase(0, 1);
		}
		else
			tokenWord(line);	
	}
}

std::vector<TOKEN_PAIR> TokenConfig::TokenTheConfig(std::ifstream &file){

	std::string line;

	while (!file.eof()){
		std::getline(file, line, '\n');
		this->tokenLine(line);
	}
	return this->tokens;	
}
