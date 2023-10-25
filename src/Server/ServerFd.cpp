
#include "../../include/Server.hpp"
#include "../../include/GenerateLocation.hpp"
#include "../../include/Global.hpp"
#include <sys/poll.h>
#include <sys/socket.h>


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

	int socket_fd = socket(MyAddr->ai_family , MyAddr->ai_socktype , 0);
	if (socket_fd == -1){
		std::cerr << "Socket : failed " << std::endl;
		freeaddrinfo(MyAddr);
		return false;
	}
	if (bind(socket_fd, MyAddr->ai_addr,MyAddr->ai_addrlen) != 0){
		std::cerr << "Socket : failed " << std::endl;
		freeaddrinfo(MyAddr);
		return false;
	}
	freeaddrinfo(MyAddr);
	if (listen(socket_fd, SOMAXCONN) == -1){
		std::cerr << "listen : failed " << std::endl;
		return false;
	}
	_listen = socket_fd;
	Global::insertFd(_listen);
	return true;
}

bool Server::isMyFd(int fd){
	if (fd == _listen)
		return true;

	for(unsigned int i=0; i < clients.size(); i++){
		if (fd == clients[i]->getFd())
			return true;
	}
	return false;
}

void Server::handelFd(struct pollfd pfd){
	if (pfd.fd == _listen){
		// accept(int, struct sockaddr *, socklen_t *);	
	}
	else {
		//TODO : Find the client and call readreq 
	}
		
}
