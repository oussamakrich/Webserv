
#include "../include/Server.hpp"
#include "../../Parsing/include/GenerateLocation.hpp"
#include "../include/Global.hpp"


bool Server::start(){
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

	_listen = socket(MyAddr->ai_family , MyAddr->ai_socktype , 0);
	if (_listen == -1){
		std::cerr << "Socket : failed " << std::endl;
		freeaddrinfo(MyAddr);
		return false;
	}
	if (bind(_listen, MyAddr->ai_addr,MyAddr->ai_addrlen) != 0){
		std::cerr << "Socket : failed " << std::endl;
		freeaddrinfo(MyAddr);
		return false;
	}
	freeaddrinfo(MyAddr);
	if (listen(_listen, SOMAXCONN) == -1){
		std::cerr << "listen : failed " << std::endl;
		return false;
	}
	Global::insertFd(_listen);
	return true;
}

bool Server::handelFd(struct pollfd pfd){
	int clientFd;
	Client	*newClient;
	if (pfd.fd == _listen){
		//FIX : addr of client is NULL for now
		clientFd = accept(_listen, NULL, NULL);
		if (clientFd  == -1){
			std::cerr << "ERROR : Connection failed" << std::endl;
			return true;
		}
		// string res= ; //"HTTP/1.1 200 OK\r\n\r\nservername: " + serverName;
		stringstream head;
		head <<  "HTTP/1.1 200 OK\n" << "Content-Length: " << serverName.length() <<  "\n" <<  "Content-Type: text/html\n" <<  "Connection: Closed\n\n" <<  serverName;
		send(clientFd, head.str().c_str(), head.str().size(), 0);

		std::cout << serverName + " : new connection accepted" << std::endl;
		newClient = new Client;
		newClient->setFd(clientFd);
		this->clients.push_back(newClient);
		Global::insertFd(clientFd);
		// close(clientFd)
	}
	else {
		//TODO : Find the client and call readreq
		for(unsigned int i=0; i < clients.size(); i++){
			if (pfd.fd == clients[i]->getFd()){
				close(clients[i]->getFd());

			}
		}
	}
	return false;
}
