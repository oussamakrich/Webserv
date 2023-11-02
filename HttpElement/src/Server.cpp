
#include "../include/Server.hpp"
#include "../../Parsing/include/GenerateLocation.hpp"
#include "../include/Global.hpp"

Server::Server(){
	this->host = "localhost";
	this->port = 80;
	this->clientMaxBodySize = 1;
	this->autoIndex = false;
}

Server::~Server(){}

Server::Server(const Server &copy){*this = copy;}

Server &Server::operator=(const Server &copy)
{
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
		autoIndex = copy.autoIndex;
		locations = copy.locations;
		_listen = copy._listen;
		clients	= copy.clients;

		
	return *this;
}

void Server::final()
{
	if (this->defaultType.empty())
		this->defaultType = "application/octet-stream";
	index.shrink_to_fit();
	errorPages.shrink_to_fit();
	CheckRepeat.shrink_to_fit();
}

void Server::setPort(int port) { this->port = port; }

void Server::setClientMaxBodySize(int size) { this->clientMaxBodySize = size; }

void Server::setAutoIndex(bool autoIndex) { this->autoIndex = autoIndex; }

void Server::setHost(const std::string& host) { this->host = host; }

void Server::setRoot(const std::string& root) { this->root = root; }

void Server::setServerName(const std::string& name) { this->serverName = name; }

void Server::setIndex(const std::vector<std::string>& index) { this->index = index; }

void Server::setErrorPages(const VECT_ERRORPIR& errorPages) { this->errorPages = errorPages; }

void Server::setErrorPage(const ERRPAGE_PAIR errorPage) {this->errorPages.push_back(errorPage);}

void Server::setDefaultType(const std::string& defaultType) { this->defaultType = defaultType; }

void Server::setAccessLog(const std::string& accessLog) { this->accessLog = accessLog; }

void Server::setErrorLog(const std::string& errorLog) { this->errorLog = errorLog; }

void Server::setMimeType(const TYPES_MAP& mimeType) { this->mimeType = mimeType; }

bool Server::setMimeType(const TYPES_PAIR& mimeType){ return this->mimeType.insert(mimeType).second;}

void Server::setLocations(const LOCATION_MAP& locations) { this->locations = locations; }

void Server::setSingleLocation(const LOCATION_PAIR& location) {this->locations.insert(location);}

void Server::setItIndex(str_it begin, str_it end){ index.insert(index.end(), begin, end);}

//INFO : +++++++++++ implementation of getters++++++++++++++

int		Server::getPort() const{return port;}

int		Server::getListen() const	{return _listen;}

bool Server::getAutoIndex() const{return autoIndex;}

int		Server::getClientMaxBodySize() const{return clientMaxBodySize;}

std::string Server::getErrorLog() const{return errorLog;}

std::string	Server::getHost() const{return host;}

std::string	Server::getServerName() const{return serverName;}

std::string Server::getRoot() const{return root;}

std::string Server::getDefaultType() const{return defaultType;}

std::string Server::getAccessLog() const{return accessLog;}

std::vector<std::string> Server::getIndex() const{return index;}

std::vector<ERRPAGE_PAIR> Server::getErrorPages() const{return errorPages;}

Location	&Server::getLocation(std::string url){ return *locations[url]; }

std::map<std::string, Location*> Server::getAllLocation() const { return locations; }

std::map<std::string, std::string>	Server::getMimeType() const{return mimeType;}


//INFO : ++++++++++++++++++++++++++for Print++++++++++++++++++++++++++++++++++++

std::ostream &operator<<(std::ostream &out, const Server &server){
	out << BLUE "serverName: " << U_YELLOW << server.getServerName() << std::endl;
	out << RED "\tport: " << GREEN << server.getPort() << std::endl;
	out << RED "\tclientMaxBodySize: " <<GREEN <<  server.getClientMaxBodySize() << std::endl;
	out << RED "\thost: " <<GREEN <<  server.getHost() << std::endl;
	out << RED "\troot: " <<GREEN <<  server.getRoot() << std::endl;
	out << RED "\tserverName: " <<GREEN <<  server.getServerName() << std::endl;
	out << RED "\tindex: " << GREEN;
	for (size_t i = 0; i < server.getIndex().size(); i++)
		out << server.getIndex()[i] << " ";
	out << std::endl;
	out << RED"\terrorPages: "<< RESET;
	for (size_t i = 0; i < server.getErrorPages().size(); i++)
		out << server.getErrorPages()[i].first << " " << server.getErrorPages()[i].second << " ";
	out << std::endl;
	out << RED"\tdefaultType: " << GREEN << server.getDefaultType() << std::endl;
	out << RED"\taccessLog: " << GREEN << server.getAccessLog() << std::endl;
	out << RED"\terrorLog: "  << GREEN<< server.getErrorLog() << std::endl;
	out << RED"\tmimeType: " << std::endl;

	std::map<std::string, std::string> mime = server.getMimeType();
	for (std::map<std::string, std::string>::const_iterator it = mime.begin(); it != mime.end(); it++)
		out << GREEN"\t\t"<< it -> first << " " << it -> second << std::endl;
	out << RED"\tlocations: " << std::endl;
	std::cout << RESET;
	std::map<std::string, Location*> loc = server.getAllLocation();
	for (std::map<std::string, Location*>::const_iterator it = loc.begin(); it != loc.end(); it++)
	{
		out <<it -> first << " " ;
		it -> second->printLocation();
		std::cout << std::endl;
	}
	return out;
}


