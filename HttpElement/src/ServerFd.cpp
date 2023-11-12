
#include "../include/Server.hpp"
#include "../include/Global.hpp"
#include "../../Response/include/GenerateResponse.hpp"
#include "../../Uploader/include/Upload.hpp"

bool creatSocket(int *listen, addrinfo *MyAddr){
	*listen = socket(MyAddr->ai_family , MyAddr->ai_socktype , 0);
	if (*listen == -1){
		std::cerr << "Socket : failed " << std::endl;
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

	stringstream PortString;
	PortString << this->port;
	struct addrinfo hints;
	bzero(&hints, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *MyAddr;
	int	ret = getaddrinfo(this->getHost().c_str(), PortString.str().c_str(), &hints, &MyAddr);
	if(ret){
		gai_strerror(ret);
		freeaddrinfo(MyAddr);
		return false;
	}
	if (!creatSocket(&_listen, MyAddr))
		return false;

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
	// std::cout << serverName + " : new connection accepted" << std::endl;
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
	clients.erase(it);
	Global::removeFd(client->getFd());
	delete client;
}

bool Server::handelClient(ITT_CLIENT it){
	if (it == clients.end())	return false;

	Client *client = *it;
	client->setLastTime(time(NULL));
	if (client->IhaveUpload)
		client->ClientUpload(*this);
	else if (client->IhaveCGI && !client->CGIFinish)
		client->CgiRequest(it, *this);
	else if (client->IhaveResponse)
		client->OldRequest(it, *this);
	else if (!client->NewRequest(*this) && !client->response->errorInSend){
		client->response->sendErrorResponse(client->getFd());
		closeConnection(it);
		return true;
	}
	if (client->response && client->response->errorInSend){
		closeConnection(it);
		return true;
	}
	if (!client->keepAlive && !client->IhaveResponse)
		closeConnection(it);
	return true;
}

bool Server::handelFd(struct pollfd pfd){

	if (pfd.fd == _listen){
		if (pfd.revents & POLLOUT)
			return false;
		this->acceptClient();
		return true;
	}
	else
		 return this->handelClient(findClient(pfd));
}

void Server::checkTimeOut(){
	ITT_CLIENT it = clients.begin();
	Client *client;

	while (it != clients.end()){
		client = *it;
		std::time_t tm = client->getLastTime();
		std::time_t now = std::time(NULL);
		if(now - tm >= TIME_OUT){
	 		std::cout << "Client fd: " << client->getFd()  << "TIME : " << now - tm<< "\n";
			exit(0);
			closeConnection(it);
			continue;
		}
		it++;
	}
}
