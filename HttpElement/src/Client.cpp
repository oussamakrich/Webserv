
#include "../include/Client.hpp"
#include "../include/Global.hpp"

Client::Client(int bodySize, int fd) : reqBuff(bodySize){
	lastTime = std::time(NULL);
	IhaveResponse = false;
	this->response = NULL;
	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
}

Client::~Client(){
	delete this->response;
}

int Client::getFd(){
	return pfd.fd;
}

void Client::ReadRequest(){
	
	char *buffer;

	buffer = new char[5024];
	memset(buffer, 0, 5024);
	status = recv(pfd.fd, buffer, 5024, 0);
	if (status == -1 || status == 0)
	{
		delete [] buffer;
		return;
	}
	
	int level = reqBuff.insertBuffer(buffer, status);
	delete buffer;
}

bool Client::isRequestAvailable(){
	if (status == -1)
		return true;
	int check = reqBuff.getLevel();
	if (check)
		return true;
	return false;
}

Request *Client::getRequest(){
	Request *req;

	req = ParsRequest::Pars(this->reqBuff);

	return req;
}

struct sockaddr &Client::getAddr(){
	return sockaddr;
}
void Client::setAddr(struct sockaddr &addr){
	sockaddr = addr;
}

std::time_t Client::getLastTime(){ return lastTime;}

void Client::setLastTime(std::time_t tm){ this->lastTime = tm;}

void Client::switchEvent(int fd, int Flag){
	pfd.events = Flag;
	Global::switchEvent(fd, Flag);
}
