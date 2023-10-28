
#include "../include/Client.hpp"
#include <sys/socket.h>

Client::Client(int bodySize, int fd) {//: reqBuff(bodySize){
	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
}

int Client::getFd(){
	return pfd.fd;
}

void Client::ReadRequest(){
	
	char *buffer;

	// while(1){
		buffer = new char[1024];
		status = recv(pfd.fd, buffer, 1024, 0);
		if (status == -1 || status == 0)
		{
			delete buffer;
			return;
		}
		std::cout << buffer << std::endl;
		// if (reqBuff.insertToBuffer(buffer, status) > 3){
		// 	delete buffer;
		// 	return;
		// }
		delete buffer;
	// }
}

bool Client::isRequestAvailable(){

	// int check = reqBuff.getLevel();
	// if (check > 3)
	// 	return true;
	return false;
}

Request Client::getRequest(){
	Request req;

	// ParseRequest::Parse(this->reqBuff);

	return req;
}
