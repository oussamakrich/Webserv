
#include "../include/Server.hpp"
#include "../../Parsing/include/GenerateLocation.hpp"
#include "../include/Global.hpp"
#include "../../Utils/include/DirListing.hpp"
#include "../../ErrorResponse/include/GenerateError.hpp"
#include "../../Response/include/GenerateResponse.hpp"

void fn(){
	system("leaks webserv");
}

bool Server::start(){
	stringstream PortString;
	PortString << this->port;
	struct addrinfo hints;
	bzero(&hints, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *MyAddr;
	// atexit(fn);
	int	ret = getaddrinfo(this->getHost().c_str(), PortString.str().c_str(), &hints, &MyAddr);
	if(ret){
		gai_strerror(ret);
		freeaddrinfo(MyAddr);
		return false;
	}

	_listen = socket(MyAddr->ai_family , MyAddr->ai_socktype , 0);
	if (_listen == -1){
		std::cerr << "Socket : failed " << std::endl;
		freeaddrinfo(MyAddr);
		return false;
	}
	int opt = 1;
  if (setsockopt(_listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
      perror("setsockopt");
			return false;
  }
	if (bind(_listen, MyAddr->ai_addr,MyAddr->ai_addrlen) != 0){
		std::cerr << "bind : failed " << std::endl;
		freeaddrinfo(MyAddr);
		return false;
	}
	freeaddrinfo(MyAddr);
	if (listen(_listen, SOMAXCONN) == -1){
		std::cerr << "listen : failed " << std::endl;
		return false;
	}
	fcntl(_listen, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	Global::insertFd(_listen);
	return true;
}

void Server::acceptClient(){
	Client	*newClient = NULL;
	struct sockaddr sockaddr;
	bzero(&sockaddr, sizeof(sockaddr));
	socklen_t len = sizeof(sockaddr);
	int clientFd = accept(_listen, &sockaddr, &len);
	if (clientFd  == -1){
		std::cerr << "ERROR : Connection failed" << std::endl;
		return;
	}
	std::cout << serverName + " : new connection accepted" << std::endl;
	newClient = new Client(this->clientMaxBodySize, clientFd);
	newClient->setAddr(sockaddr);
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

void Server::closeConnection(ITT_CLIENT it){
	Client *client = *it;
	// close(client->getFd());
	clients.erase(it);
	Global::removeFd(client->getFd());
	delete client;
}

bool Server::handelClient(ITT_CLIENT it){
	if (it == clients.end())	return false;

	Client *client = *it;
	client->setLastTime(time(NULL));
	if (client->IhaveResponse)
	{
		client->response->ReminderResponse();
		client->IhaveResponse = client->response->stillSend;
		if (!client->IhaveResponse)
		{
			delete  client->response;
			client->response = NULL;
			client->switchEvent(client->getFd(), POLLIN);
		}
		return true;
	}
	Request *req;
	client->ReadRequest();
	if (!client->isRequestAvailable())
		return true;
	client->switchEvent(client->getFd(), POLLOUT);
	req = client->getRequest();
	client->reqBuff.clear();
	if (req->getType() > 1)
	{
		ErrorResponse err = GenerateError::generateError(req->getType(), *this);
		std::string error =  err.getErrorPage(*this);
		send(client->getFd(), error.c_str(), error.size(), 0);
		closeConnection(it);
	}
	else
	{
		client->response = GenerateResponse::generateResponse(*this, *req, client->getFd());
		client->response->sendResponse();
		client->IhaveResponse = client->response->stillSend;
		if (!client->IhaveResponse){
			delete  client->response;
			client->response = NULL;
			client->switchEvent(client->getFd(), POLLIN);
		}
	}
	delete req;
	return true;
}

bool Server::handelFd(struct pollfd pfd){

	if (pfd.fd == _listen){
		this->acceptClient();
		return true;
	}
	else
	{
		 return this->handelClient(findClient(pfd));
		std::cout << "send it" << std::endl;
	}
}

void Server::checkTimeOut(){
	ITT_CLIENT it = clients.begin();
	Client *client;
	for (; it != clients.end(); it++){
		client = *it;
		std::time_t tm = client->getLastTime();
		if(tm - std::time(NULL) >= TIME_OUT){
			std::cout << "client deleted" << std::endl;
			closeConnection(it);
		}
		// else
		// 	std::cout << "client not deleted" << std::endl;
	}
}
