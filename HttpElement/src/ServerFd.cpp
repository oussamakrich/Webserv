
#include "../include/Server.hpp"
#include "../../Utils/include/DirListing.hpp"
#include "../include/Global.hpp"
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
	std::cout << "client added : " << clientFd << std::endl;
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
	// if (client->IhaveResponse){
		// if (client->response->ReminderResponse()){
		// 	client->response->sendErrorResponse(*this, client->getFd());
		// 	closeConnection(it);
		// 	return true;
		// }
		// client->IhaveResponse = client->response->stillSend;
		// if (!client->IhaveResponse){
		// 	delete  client->response; 
		// 	client->response = NULL;
		// 	client->switchEvent(client->getFd(), POLLIN);
		// }
		// return true;
	// }	
	// Request *req;
	// client->ReadRequest();
	// if (!client->isRequestAvailable())
	// 	return true;
	// client->switchEvent(client->getFd(), POLLOUT);
	// req = client->getRequest();
	// client->reqBuff.clear();
	// if (req->getType() < 0)
	// {
	// 	client->response->setCode(req->getType());
	// 	client->response->sendErrorResponse(*this, client->getFd());
	// 	delete req;
	// 	closeConnection(it);
	// 	return true;
	// }
	// else{
	// 	client->response = GenerateResponse::generateResponse(*this, *req, client->getFd());
	// 	if (!client->response->sendResponse()){
	// 		client->response->sendErrorResponse(*this, client->getFd());
	// 		delete req;
	// 		closeConnection(it);
	// 		return true;
	// 	}
	// 	client->IhaveResponse = client->response->stillSend;
	// 	if (!client->IhaveResponse){
	// 		delete  client->response; 
	// 		client->response = NULL;
	// 		client->switchEvent(client->getFd(), POLLIN);
	// 	}
	// }
	// delete req;
	if (client->IhaveResponse)
		return client->OldRequest(it, *this);
	if (!client->NewRequest(it, *this))
	{
		client->response->sendErrorResponse(*this, client->getFd());
		closeConnection(it);

	}
	return true;
}

bool Server::handelFd(struct pollfd pfd){

	if (pfd.fd == _listen){
		if (pfd.revents & POLLOUT){
			std::cout << "listen fd is writable" << std::endl;
			return false;
		}
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
		std::time_t now = std::time(NULL);
		// std::cout << " time-out : " << now - tm  << " : " << TIME_OUT<< std::endl;
		if(now - tm >= TIME_OUT){
			std::cout << "client deleted" << std::endl;
			closeConnection(it);
		}
		if (clients.size() == 0)
			return;
	}
}
