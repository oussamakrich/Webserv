
#include "../include/Server.hpp"
#include "../../Parsing/include/GenerateLocation.hpp"
#include "../include/Global.hpp"

Server::Server(){
	this->host = "0.0.0.0";
	this->port = 80;
	this->clientMaxBodySize = 1000000;
	this->autoIndex = false;
	this->listenRepeat = false;
	this->ServerOff = false;
}

Server::~Server()
{
	for (std::map<std::string, Location*>::iterator it = locations.begin(); it != locations.end(); it++)
		delete it->second;
	locations.clear();
}

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
		mimeType = copy.mimeType;
		autoIndex = copy.autoIndex;
		locations = copy.locations;
		_listen = copy._listen;
		clients	= copy.clients;


	return *this;
}

TYPES_MAP generateMimeType(){
	TYPES_MAP mime;

	mime["html"]		= "text/html";
  mime["htm"]		= "text/html";
  mime["shtml"]	= "text/html";
  mime["css"]		= "text/css";
  mime["xml"]		= "text/xml";
  mime["gif"]		= "image/gif";
  mime["jpeg"]		= "image/jpeg";
  mime["jpg"]		= "image/jpeg";
  mime["js"]			= "application/javascript";
  mime["txt"]		= "text/plain";
  mime["avif"]		= "image/avif";
  mime["png"]		= "image/png";
  mime["svg"]		= "image/svg+xml";
  mime["json"]		= "application/json";
  mime["pdf"]		= "application/pdf";
  mime["rar"]		= "application/x-rar-compressed";
  mime["zip"]		= "application/zip";
  mime["bin"]		= "application/octet-stream";
  mime["exe"]		= "application/octet-stream";
  mime["iso"]		= "application/octet-stream";
  mime["img"]		= "application/octet-stream";

	return mime;
}

void Server::final()
{
	if (this->defaultType.empty())
		this->defaultType = "application/octet-stream";
	if (this->mimeType.empty())
		this->mimeType = generateMimeType();
	index.shrink_to_fit();
	errorPages.shrink_to_fit();
	CheckRepeatErrorPages.shrink_to_fit();
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

std::string	Server::getHost() const{return host;}

std::string	Server::getServerName() const{return serverName;}

std::string Server::getRoot() const{return root;}

std::string Server::getDefaultType() const{return defaultType;}

std::vector<std::string> Server::getIndex() const{return index;}

std::vector<ERRPAGE_PAIR> Server::getErrorPages() const{return errorPages;}

Location	&Server::getLocation(std::string url){ return *locations[url]; }

LOCATION_MAP &Server::getAllLocation() { return locations; }

std::map<std::string, std::string>	Server::getMimeType() const{return mimeType;}


//INFO : ++++++++++++++++++++++++++for Print++++++++++++++++++++++++++++++++++++

std::ostream &operator<<(std::ostream &out, Server &server){
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
	out << RED"\tmimeType: " << std::endl;

	std::map<std::string, std::string> mime = server.getMimeType();
	for (std::map<std::string, std::string>::const_iterator it = mime.begin(); it != mime.end(); it++)
		out << GREEN"\t\t"<< it -> first << " " << it -> second << std::endl;
	out << RED"\tlocations: " << std::endl;
	std::cout << RESET;
	LOCATION_MAP loc = server.getAllLocation();
	for (std::map<std::string, Location*>::const_iterator it = loc.begin(); it != loc.end(); it++)
	{
		out <<it -> first << " " ;
		it -> second->printLocation();
		std::cout << std::endl;
	}
	return out;
}
