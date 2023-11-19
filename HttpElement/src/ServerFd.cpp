
#include "../include/Server.hpp"
#include "../include/Global.hpp"
#include "../../Response/include/GenerateResponse.hpp"
#include "../../Uploader/include/Upload.hpp"
#include "../../Utils/include/Logger.hpp"
#include <cstdio>

addrinfo *addrInfo(std::string host, int port){

	addrinfo *MyAddr;
	stringstream PortString;
	PortString << port;
	struct addrinfo hints;
	bzero(&hints, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
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

void Server::acceptClient(){

	Client	*newClient = NULL;
	int clientFd = accept(_listen, NULL, NULL);
	if (clientFd  == -1){
		perror("accept");
		return;
	}
	fcntl(clientFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	newClient = new(std::nothrow) Client(this->clientMaxBodySize, clientFd);
	if (newClient == NULL){
		std::cerr << "ERROR : new Client failed" << std::endl;
		return;
	}
	newClient->id = generateId();//Debug
	newClient->time = getTime();//Debug
	Logger::fastLog(Logger::INFO, "./Log/" + (newClient->id),  "Accept new client");
	std::cout << "Accept new client with the id: " << newClient->id << std::endl;
	this->clients.push_back(newClient);
	Global::insertFd(clientFd);
}

ITT_CLIENT Server::findClient(pollfd pfd){

	ITT_CLIENT it = clients.begin();
	for(; it != clients.end(); it++)
		if (pfd.fd == (*it)->getFd())
				break ;
	return it;
}

bool Server::handelFd(struct pollfd pfd){

	if (pfd.fd == _listen){
		if (pfd.revents & POLLOUT)
			return false;
		this->acceptClient();
		return true;
	}
	else
		 return this->handelClient(findClient(pfd), pfd);
}

void Server::closeConnection(ITT_CLIENT it){
	Client *client = *it;
	Logger::fastLog(Logger::INFO, "./Log/" + client->id,  "client closed the connection");
	clients.erase(it);
	Global::removeFd(client->getFd());
	delete client;
}

bool Server::handelClient(ITT_CLIENT it, pollfd pfd){
	if (it == clients.end())	return false;

	Client *client = *it;
	Global::id = client->id;//Debug
	Global::time = client->time;//Debug
	if (pfd.revents & POLLHUP){
		// Logger::fastLog(Logger::INFO, "./Log/" + client->id,  "revents is POLLHUP: ");
		client->clearClient();	
		this->closeConnection(it);
		return true;
	}
	client->setLastTime(time(NULL));
	// Logger::fastLog(Logger::INFO, "./Log/" + client->id,  "set last time: " + convertCode((client->getLastTime())));
	if (client->IhaveUpload)
		client->ClientUpload(*this);
	else if (client->IhaveCGI && !client->CGIFinish){
		// Logger::fastLog(Logger::INFO, "./Log/" + client->id,  "Ihave cgi and cgiFinish false");
		client->CgiRequest();
	}
	else if (client->IhaveResponse){
		if (client->OldRequest() == false)
			closeConnection(it);
	}
	else if (!client->NewRequest(*this)){
		client->response->sendErrorResponse(client->getFd(), client->keepAlive);
		client->resetClient();
		client->switchEvent(pfd.fd, POLLIN);
		return true;
	}
	client->setLastTime(time(NULL));
	if (!client->keepAlive && !client->IhaveResponse){
		std::cout <<"KeepAlive is false && Idont have resp" << std::endl;
		closeConnection(it);
	}
	return true;
}

void Server::checkTimeOut(){
	ITT_CLIENT it = clients.begin();
	Client *client;

	while (it != clients.end()){
		client = *it;
		std::time_t tm = client->getLastTime();
		std::time_t now = std::time(NULL);
		if(now - tm >= TIME_OUT){
			Global::id = client->id;
			Global::time = client->time;
	 		std::cout << "Client fd: " << client->getFd()  << " TIME : " << now - tm<< "\n";
			Logger::fastLog(Logger::INFO, "./Log/" + client->id,  "client timeout");

			closeConnection(it);
			continue;
		}
		it++;
	}
}
