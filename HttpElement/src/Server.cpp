
#include "../include/Server.hpp"
#include "../../Parsing/include/GenerateLocation.hpp"
#include "../include/Global.hpp"

Server::Server(){
	this->host = "0.0.0.0";
	this->port = 80;
	this->clientMaxBodySize = 1000000;
	this->autoIndex = false;
	this->listenRepeat = false;
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
  mime["mp4"]		= "video/mp4";

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

void Server::setClientMaxBodySize(size_t size) { this->clientMaxBodySize = size; }

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

bool Server::setSingleLocation(const LOCATION_PAIR& location) {return  this->locations.insert(location).second;}

void Server::setItIndex(str_it begin, str_it end){ index.insert(index.end(), begin, end);}

//INFO : +++++++++++ implementation of getters++++++++++++++

int		Server::getPort() const{return port;}

int		Server::getListen() const	{return _listen;}

bool Server::getAutoIndex() const{return autoIndex;}

size_t		Server::getClientMaxBodySize() const{return clientMaxBodySize;}

std::string	Server::getHost() const{return host;}

std::string	Server::getServerName() const{return serverName;}

std::string Server::getRoot() const{return root;}

std::string Server::getDefaultType() const{return defaultType;}

std::vector<std::string> Server::getIndex() const{return index;}

std::vector<ERRPAGE_PAIR> Server::getErrorPages() const{return errorPages;}

Location	&Server::getLocation(std::string url){ return *locations[url]; }

LOCATION_MAP &Server::getAllLocation() { return locations; }

std::map<std::string, std::string>	Server::getMimeType() const{return mimeType;}


addrinfo *addrInfo(std::string host, int port){

	addrinfo *MyAddr;
	stringstream PortString;
	PortString << port;
	struct addrinfo hints;
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int	ret = getaddrinfo(host.c_str(), PortString.str().c_str(), &hints, &MyAddr);
	if(ret){
		std::cerr << gai_strerror(ret) << std::endl;
		freeaddrinfo(MyAddr);
		return NULL;
	}
	return MyAddr;
}

bool creatSocket(int *listen, addrinfo *MyAddr){
	*listen = socket(MyAddr->ai_family , MyAddr->ai_socktype , 0);
	if (*listen == -1){
		perror("socket");
		freeaddrinfo(MyAddr);
		return false;
	}
	int opt = 1;
	if (setsockopt(*listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		return false;
	}
	return true;
}

bool Server::start(){

	if (listenRepeat == true) // NOTE : if the server is repeated dont start it
		return true;
	addrinfo *MyAddr = addrInfo(this->getHost(), this->getPort());
	if (!MyAddr)
		return false;
	if (!creatSocket(&_listen, MyAddr))
		return false;

	if (bind(_listen, MyAddr->ai_addr, MyAddr->ai_addrlen) != 0){
		perror("bind");
		freeaddrinfo(MyAddr);
		return false;
	}
	freeaddrinfo(MyAddr);
	if (listen(_listen, SOMAXCONN) == -1){
		perror("listen");
		return false;
	}
	fcntl(_listen, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	Global::insertFd(_listen);
	return true;
}
