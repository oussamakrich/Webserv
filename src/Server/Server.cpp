
#include "../../include/Server.hpp"
#include "../../include/GenerateLocation.hpp"
#include <algorithm>
#include <mutex>

void error(std::string error){
	std::cerr << RED"Syntax Error : "<< RESET << error  << std::endl;
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

void Server::Shrink(){
		index.shrink_to_fit();
		errorPages.shrink_to_fit();
		CheckRepeat.shrink_to_fit();
}

int Server::parseErrorPage(std::string codeValue){
	if (codeValue.find_first_not_of("0123456789") != codeValue.npos) 
		error("error code should be a number in ErrorPage");
	if (codeValue.size() != 3)
		error("error code range between 400 To 599");
	int errorCode = atoi(codeValue.c_str());
	if (errorCode < 400 || errorCode > 599)
		error("error code range between 400 To 599");
	if (std::find(CheckRepeat.begin(), CheckRepeat.end(), errorCode) != CheckRepeat.end())
			error("errorCode is repeated twice in ErrorPage");
	CheckRepeat.push_back(errorCode);
	return errorCode;	
}

void Server::SetErrorPages(TOKEN_IT &it){
	ERRPAGE_PAIR pair;
	int	code;
	std::string value;

	if (it->first != WORD)	error("Expect NUMBER after ErrorPage");
	 code = this->parseErrorPage(it->second);
	it++;
	if (it->first != WORD)	error("Expect Tow value after ErrorPage");
	value = it->second;
	it++;
	if (it->first != SEMICOLON)	error("Expect only Tow value after ErrorPage");
	
	pair.first = code;
	pair.second = value;
	if (std::find(errorPages.begin(), errorPages.end(), pair) != errorPages.end())
			error("value is repeated twice in ErrorPage");
	errorPages.push_back(pair);	
}

void Server::fillLocation(TOKEN_IT &it){

	Location *location =	GenerateLocation::generateLocation(it);
	std::string uri = location->getUri();
	this->locations.insert(std::make_pair(uri, location));
}

void Server::SetTypes(TOKEN_IT &it){
	std::string value, key;

	if (it->first == OPEN_C_BRACKET)
		it++;
	if (it->first != WORD)	error("Types should be followd by WORD");
	value = it->second;
	if ((++it)->first != WORD)	error("Types Expect Key and Value");

	while (it->first != CLOSE_C_BRACKET){
		while (it->first != SEMICOLON && it->first != CLOSE_C_BRACKET){
			key = it->second;
			if (!mimeType.insert(std::make_pair(key, value)).second)
				error("duplicate Type");
			it++;
		}
		if (it->first == CLOSE_C_BRACKET)
			break;
		it++;
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

	while (it->first != SEMICOLON)
	{
		if (it->first == WORD){
			if (std::find(index.begin(), index.end(), it->second) != index.end())
				error(it->second + " is repeated twice in index");
			index.push_back(it->second);
		}
		it++;
	}
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
			error("Port is grather tha max");
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
			error("Listen '[' Should be closed");
	this->host = line.substr(0, pos + 1);
	if (this->host.find_first_not_of("ABCDEFabcdef0123456789:[]") != host.npos)
		error("Unexpected Charachter in host");
	line.erase(0, pos + 1);
	if (!line.empty()){
		if (line.front() != ':' || line.back() == ':')
			error("Listen expect ':' after host");
		portStr = line.substr(1);
		if (portStr.find_first_not_of("0123456789") != line.npos || portStr.size() > 5)
			error("In Port Value");
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
				error("Identifier repeated twice ':' ");
			this->host = line.substr(0, pos);
			portStr = line.substr(pos + 1);
			if (portStr.find_first_not_of("0123456789") != line.npos || portStr.size() > 5)
				error("In Port Value");
			this->port = atoi(portStr.c_str());
		}
	}
	if (port <= 0 || port >  USHRT_MAX)
		error("Port out of Range");
} 

void	Server::SetHostAndPort(TOKEN_IT &it){
	
	std::string value = it->second;
	it++;	
	if (it->first != SEMICOLON)
		error("Listen accept one value");
	if(value.back() == ':')
		error("':' cannot be in the back");
	if(value.front() == ':')
		error("':' cannot be in the front");
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
std::vector<ERRPAGE_PAIR> Server::getErrorPages() const{
	return errorPages;
}
std::string Server::getDefaultType() const{
	return defaultType;
}
std::string Server::getAccessLog() const{
	return accessLog;
}

Location	&Server::getLocation(std::string url) {

	return *locations[url];
}

std::map<std::string, Location*> Server::getAllLocation(){
	return locations;
}

