
#include "../../include/Server.hpp"
#include <cstdlib>

	Server::Server(){}

	Server::~Server(){}

	Server::Server(const Server &copy){*this = copy;}

	Server &Server::operator=(const Server &copy){
		port = copy.port;
		clientMaxBodySize = copy.clientMaxBodySize;
		host = copy.host ;
		root = copy.root ;
		// = copy. ;
		// = copy. ;
		// = copy. ;
	return *this;
}

void Server::SetSingleValue(TOKEN_IT &it){

	std::string value;
	Token				key = it->first;

	while (it->first != SEMICOLON)
	{
		if (it->first == WORD)
			value = it->second;
		it++;
	}
	if (key == HOST)							host = value;
	else if (key == ROOT)					root = value;
	else if (key == SERVER_NAME)	serverName = value;
	else if (key == DEFAULT_TYPE)	defaultType = value;
	else if (key == ACCESS_LOG)		accessLog = value;
	else if (key == ERROR_LOG)		errorLog = value;
	it++;
}

void Server::SetMultiValue(TOKEN_IT &it){

	Token				key = it->first;
	std::vector<std::string> values;
	while (it->first != SEMICOLON)
	{
		if (it->first == WORD)
			values.push_back(it->second);
		it++;
	}
	if (key == ERROR_PAGES)	errorPages = values;
	else if (key == INDEX)	index = values;
	it++;
}

void Server::SetPort(TOKEN_IT &it){
	short prt;
	while (it->first != SEMICOLON)
	{
		if (it->first == WORD)
			prt = atoi(it->second.c_str());
		it++;
	}
	port = prt;
	it++;
}

void Server::SetBodySize(TOKEN_IT &it){
	short size;
	while (it->first != SEMICOLON)
	{
		if (it->first == WORD)
			size = atoi(it->second.c_str());
		it++;
	}
	port = size;
	it++;
}
