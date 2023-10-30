
#include "../include/Server.hpp"
#include "../../Parsing/include/GenerateLocation.hpp"
#include "../include/Global.hpp"
#include "../../Utils/include/DirListing.hpp"
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

#include "../../ErrorResponse/include/GenerateError.hpp"

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
		std::cerr << "bind : failed " << std::endl;
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

bool Server::handelClient(ITT_CLIENT it){
	if (it == clients.end())	return false;

	Client *client = *it;
	Request req;
	client->ReadRequest();
	
	ErrorResponse err = GenerateError::generateError(501, *this);
	std::string error =  err.getErrorPage(*this);
	send(client->getFd(), error.c_str(), error.size(), 0);


	//TODO :GenerateResponse
	// DirListing dir;
	// std::string  out;
	// dir.getDirlistigHtml("/tmp", out);
	// 
	// stringstream head;
	// head << "HTTP/1.1 200 OK\nContent-Length: ";
	// head << out.length();
	// head << "\nContent-Type: text/html\nConnection: Closed\n\n";
	// head << out;
	// std::cout << head.str() << std::endl;
	// send(client->getFd(), head.str().c_str(), head.str().size(), 0);
	close(client->getFd());
	delete client;
	clients.erase(it);
	Global::removeFd(client->getFd());
	std::cout << "send iit" << std::endl;
	return true;
}

bool Server::handelFd(struct pollfd pfd){

	if (pfd.fd == _listen){
		this->acceptClient();
		return true;
	}
	else 
	{
		std::cout << "send iit" << std::endl;
		 return this->handelClient(findClient(pfd));	
	}
}
