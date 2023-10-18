#include "../../include/ParseConfig.hpp"
#include "../../include/TokenConfig.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
// Start: Canonical form:
ParseConfig::ParseConfig(){}
ParseConfig::~ParseConfig(){}
ParseConfig &ParseConfig::operator=(ParseConfig const &copy){
	if (this != &copy){}
	return *this;
}
ParseConfig::ParseConfig(ParseConfig const &copy){
	*this = copy;
}
// End: Canonical form

void ParseConfig::FillMimeTypes() {
	std::string	line;

	std::cout << mimeTypeFile << std::endl;
	std::ifstream mimeFile(mimeTypeFile);
	if (!mimeFile.is_open())
		throw std::runtime_error("Error: Mime file not found.");
	while (std::getline(mimeFile, line))
	{
		if (line.empty())
			continue;
		ss.clear();
		ss.str(line);
		newMime = true;
		while (std::getline(ss, line, ' '))
		{
			if (line.empty())
				continue ;
			if (newMime)
			{
				value = line;
				newMime = false;
			}
			else {
				if (*(line.end() - 1) == ';')
					key = line.substr(0, line.find(';'));
				else
					key = line;
				mimeTypes[key] = value;
			}
		}
	}
}

void ParseConfig::getMimeFile(std::vector<TOKEN_PAIR>::iterator it){

	for (;it != tokens.end(); it++){
		if (it->second == VALUE)
		{
			this->mimeTypeFile = it->first;
			break ;
		}
	}
}
void ParseConfig::checkBeforToken(std::vector<TOKEN_PAIR>::iterator it){
	if (it == this->tokens.begin())
		throw std::runtime_error("ERROR: COLONE and SEMICOLONE cannot be in the front");
	it--;
	if (it->second != VALUE)
		throw std::runtime_error("ERROR: COLONE and SEMICOLONE should be preceded by VALUE");
}

TOKEN_PAIR ParseConfig::skipSpaces(std::vector<TOKEN_PAIR>::iterator &it){

	for (;it != tokens.end(); ++it){
		if (it->second != ESP)
			break;
	}
	return *it;
}

void ParseConfig::checkToken(std::vector<TOKEN_PAIR>::iterator &it, token type){
	TOKEN_PAIR pair;

	pair = skipSpaces(++it);
	if (pair.second != type)
		throw std::runtime_error(ERR_CONFIGFILE);
}


void ParseConfig::closed(){
	int bracketClosed = 0;
	int curlyClosed = 0;
	std::vector<TOKEN_PAIR>::iterator it;
	for (it = tokens.begin(); it != tokens.end(); ++it){
		TOKEN_PAIR pair = skipSpaces(it);
		if (pair.second == BRACKETOPEN)
			bracketClosed++;
		if (pair.second == BRACKETCLOSE)
			bracketClosed--;
		if (pair.second == CURLYOPEN)
			curlyClosed++;
		if (pair.second == CURLYCLOSE)
			curlyClosed--;
		if (curlyClosed < 0 || bracketClosed < 0)
			throw std::runtime_error(UNCLOSED);
	}
	if (curlyClosed != 0 || bracketClosed != 0)
		throw std::runtime_error(UNCLOSED);
}

void ParseConfig::SyntaxError(){
	std::vector<TOKEN_PAIR>::iterator it;
	this->closed();
	for (it = tokens.begin(); it != tokens.end(); it++){
		TOKEN_PAIR pair = skipSpaces(it);
		if (pair.second == INCLUDE){
			this->checkToken(it, COLONE);
			this->getMimeFile(it);
		}
		else if (pair.second == SERVER)
		{
			this->checkToken(it, BRACKETOPEN);
		}
		else if (pair.second == LOCATION)
		{
			this->checkToken(it, VALUE);
			this->checkToken(it, CURLYOPEN);
		}
		else if (pair.second == BRACKETCLOSE){
			pair = skipSpaces(++it);
			if (pair.second != SERVER && pair.second != END)
				throw std::runtime_error(ERR_CONFIGFILE);
			it--;
		}
		else if (pair.second == BRACKETOPEN || pair.second == CURLYOPEN)
			throw std::runtime_error(ERR_CONFIGFILE);
		else if (pair.second == COLONE || pair.second == SEMICOLONE)
			this->checkBeforToken(it);
	}
	std::cout << "Syntax Valid"<< std::endl;
}

std::vector<Server*> &ParseConfig::getServers()
{
	this->generator.Generator(this->tokens, this->servers);
	return this->servers;
}

ParseConfig::ParseConfig(std::string &path){

	std::string fileString;
	std::ifstream configFile(path);
	TokenConfig TokenConfig;

	if (!configFile.is_open())
		throw std::runtime_error("ERR_OPEN");

	this->tokens = TokenConfig.TokenTheConfig(configFile);
	configFile.close();

	this->SyntaxError();
	this->FillMimeTypes();

}
