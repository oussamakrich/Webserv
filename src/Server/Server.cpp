
#include "../../include/Server.hpp"
#include <cstdlib>
#include <string>
#include <utility>

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


void Server::SetTypes(TOKEN_IT &it){
	std::string value, key;

	if (it->first == OPEN_C_BRACKET)
		it++;
	for(;it->first != CLOSE_C_BRACKET; it++){
		value = it->second;
		it++;
		while(it->first != SEMICOLON){
			key = it->second;	
			mimeType.insert(std::make_pair(key, value));
			it++;
		}
	}
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
	if (key == SERVER_NAME)	serverName = value;
	else if (key == DEFAULT_TYPE)	defaultType = value;
	else if (key == ACCESS_LOG)		accessLog = value;
	else if (key == ERROR_LOG)		errorLog = value;
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
}

void Server::SetInt(TOKEN_IT &it){
	int		size;
	Token key = it->first;
	while (it->first != SEMICOLON)
	{
		if (it->first == WORD)
			size = atoi(it->second.c_str());
		it++;
	}
	if (key == MAX_BODY_SIZE)	clientMaxBodySize = size;
	else if (key == PORT)	port = size;
}

void	Server::SetHostAndPort(TOKEN_IT &it){

	std::string hostAndPort, Sport;
	size_t			pos;
	

	hostAndPort = it->second;
	pos = hostAndPort.find(':');
	if (pos != hostAndPort.rfind(':')){
		std::cerr << "Syntax Error: listen should take only one ':'" << std::endl;
		return	;
	}
	if (pos != hostAndPort.npos){
		host = hostAndPort.substr(0, pos);
		Sport = hostAndPort.substr(pos, pos - hostAndPort.size());
	}
	else{
		
	}
	
		
}


//INFO : +++++++++++ implementation of getters++++++++++++++

int		Server::getPort() const{
		return port;
}
int			Server::getClientMaxBodySize() const{
	return clientMaxBodySize;
}
std::string Server::getErrorLog() const{
	return errorLog;
}
std::vector<std::string> Server::getIndex() const{
	return index;
}
std::vector<std::string> Server::getErrorPages() const{
	return errorPages;
}
std::string Server::getDefaultType() const{
	return defaultType;
}
std::string Server::getAccessLog() const{
	return accessLog;
}
