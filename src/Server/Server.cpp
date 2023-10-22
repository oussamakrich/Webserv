
#include "../../include/Server.hpp"
#include <climits>
#include <cstdlib>

void error(std::string error){
	std::cerr << error << std::endl;
	exit(1);
}

	Server::Server(){
	this->host = "localhost";
	this->port = 80;
	this->defaultType = "application/octet-stream";
	this->clientMaxBodySize = 1;
}

	Server::~Server(){}

	Server::Server(const Server &copy){*this = copy;}

	Server &Server::operator=(const Server &copy){
		port = copy.port;
		clientMaxBodySize = copy.clientMaxBodySize;
		host = copy.host ;
		root = copy.root ;
		serverName = copy.serverName;
		index = copy.index;
		errorPages = copy.errorPages;
		defaultType = copy.defaultType;
		accessLog = copy.accessLog;
		errorLog = copy.errorLog;
		mimeType = copy.mimeType;
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

	std::string value, keyWord;
	Token				key = it->first;

	keyWord = it->second;
	it++;
	if (it->first != WORD)
		error("Error: " + keyWord + " Should be followed by WORD");
	value = it->second;
	it++;
	if (it->first != SEMICOLON)
		error("Error: " + keyWord +" Accept only one Value");
	if (key == SERVER_NAME)				serverName = value;
	else if (key == DEFAULT_TYPE)	defaultType = value;
	else if (key == ACCESS_LOG)		accessLog = value;
	else if (key == ERROR_LOG)		errorLog = value;
	else if (key == ROOT)					root = value;
	
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
	while (it->first != SEMICOLON)
	{
		if (it->first == WORD)
			size = atoi(it->second.c_str());
		it++;
	}
	clientMaxBodySize = size;
}


void	Server::handelOne(std::string line){
	size_t pos= line.find_first_not_of("0123456789");
	if (pos == line.npos){
		if (line.size() > 5)
			error("Error: port is grather tha max");
		this->port = atoi(line.c_str());
	}
	else
		this->host = line;
}	

void Server::hostV6(std::string line)
{
	std::string portStr;
	size_t pos = line.find(']');	

	if (pos == line.npos)
			error("Error : listen '[' Should be closed");
	this->host = line.substr(0, pos + 1);
	if (this->host.find_first_not_of("ABCDEFabcdef0123456789:[]") != host.npos)
		error("Error: unexpected Charachter in host");
	line.erase(0, pos + 1);
	if (!line.empty()){
		if (line.front() != ':' || line.back() == ':')
			error("Error : listen expect ':' after host");
		portStr = line.substr(1);
		if (portStr.find_first_not_of("0123456789") != line.npos || portStr.size() > 5)
			error("Error: in Port Value");
		this->port = atoi(portStr.c_str());
	}
}

void Server::parseListen(std::string line){
	size_t pos;
	std::string portStr;
	if (line[0] == '[')	
		this->hostV6(line);
	else
	{
		pos = line.find(':');
		if (pos == line.npos)
			handelOne(line);
		else
		{
			if (pos !=line.rfind(':'))
				error("Error : identifier repeated twice ':' ");
			this->host = line.substr(0, pos);
			portStr = line.substr(pos + 1);
			if (portStr.find_first_not_of("0123456789") != line.npos || portStr.size() > 5)
				error("Error: in Port Value");
			this->port = atoi(portStr.c_str());
		}
	}
	if (port <= 0 || port >  USHRT_MAX)
		error("Error: port out of Range");
} 

void	Server::SetHostAndPort(TOKEN_IT &it){
	
	std::string value = it->second;
	it++;	
	if (it->first != SEMICOLON)
		error("Error: Listen accept one value");
	if(value.back() == ':')
		error("Error: ':' cannot be in the back");
	this->parseListen(value);
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
std::string	Server::getHost() const{
	return host;
}
std::string	Server::getServerName() const{
	return serverName;;
}
std::string Server::getRoot() const{
	return root;
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
