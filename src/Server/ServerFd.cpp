
#include "../../include/Server.hpp"
#include "../../include/GenerateLocation.hpp"
#include "../../include/Global.hpp"
#include <sys/socket.h>
#include <unistd.h>


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
	int clientFd;
	Client	*newClient;
	if (pfd.fd == _listen){
		//FIX : addr of client is NULL for now
		clientFd = accept(_listen, NULL, NULL);	
		if (clientFd  == -1){
			std::cerr << "ERROR : Connection failed" << std::endl;
			return;
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
		std::cout << "client re-connection accepted" << std::endl;
	}
		
}

// void  Global::run()
// {
// 	std::vector<Server *>::iterator it;
//
// 	it = servers.begin();
//
// 	for(;it != servers.end(); it++) {std::cout << (*it)->start() << std::endl;}
// 	int clientSocket;
// 	it = servers.begin();
// 	while (true)
// 	{
// 		std::cout << "polling" << std::endl;
// 		int ready = poll(this->servers[0]->getPollFds().data(), this->servers[0]->getPollFds().size(), -1);
// 		std::cout << "end polling" << std::endl;
// 		if (ready == -1) {
// 			perror("poll");
// 			break;
// 		}
// 		std::vector<pollfd> fds = servers[0]->getPollFds();
//
// 		for (unsigned int i = 0; i < fds.size(); i++) {
//
// 			if (fds[i].events & POLLIN) {
//
// 				if (fds[i].fd == this->servers[0]->getSocket()) {
// 					// Accept a new connection.
// 					clientSocket = accept(this->servers[0]->getSocket(), NULL, NULL);
// 					if (clientSocket != -1) {
// 						// Set the client socket to non-blocking mode.
// 						// fcntl(clientSocket, F_SETFL, O_NONBLOCK);
//
// 						this->servers[0]->SetFds((pollfd){clientSocket, (POLLIN | POLLOUT), 0});
// 					}
// 				} else
// 				{
//
// 					int BUFFER_SIZE = 1024;
// 					char buffer[BUFFER_SIZE + 1];
//
// 					std::cout << fds[i].fd << std::endl;
// 					int bytes = recv(fds[i].fd, buffer, BUFFER_SIZE, 0);
// 					if (bytes == -1) {
// 						perror("recv");
// 						break;
// 					}
// 					std::cout << buffer << std::endl;
// 					// close(fds[i].fd);
// 					// servers[0]->unsetFD(it);
//
//
// 				}
// 			}
// 			if (fds[i].revents & POLLOUT) {
//
// 				// Write a response to the client.
// 			}
// 		}
// 	}
// }
//
//
