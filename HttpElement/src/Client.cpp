
#include "../include/Client.hpp"
#include "../include/Global.hpp"
#include "../include/Server.hpp"
#include "../../Response/include/GenerateResponse.hpp"

#define N_READ 5024

Client::Client(int bodySize, int fd) : reqBuff(bodySize){
	lastTime = std::time(NULL);
	IhaveResponse = false;
	IhaveCGI = false;
	this->response = NULL;
	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
}

Client::~Client(){
	delete this->response;
}

int Client::getFd(){return pfd.fd; }

bool Client::ReadRequest(){

	char *buffer;
	buffer = new char[N_READ];
	memset(buffer, 0, N_READ);
	status = recv(pfd.fd, buffer, N_READ, 0);
	if (status == -1 || status == 0)
	{
		delete  buffer;
		return false;
	}
	int level = reqBuff.insertBuffer(buffer, status);
	// std::cout << buffer << std::endl;
	// exit(9);

	delete buffer;
	return true;
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

struct sockaddr &Client::getAddr(){ return sockaddr;}

void Client::setAddr(struct sockaddr &addr){ sockaddr = addr;}

std::time_t Client::getLastTime(){ return lastTime;}

void Client::setLastTime(std::time_t tm){ this->lastTime = tm;}

void Client::switchEvent(int fd, int Flag){
	pfd.events = Flag;
	Global::switchEvent(fd, Flag);
}

bool Client::OldRequest(ITT_CLIENT it, Server &ser){

	if (!response->ReminderResponse()){
		response->sendErrorResponse(ser, getFd());
		ser.closeConnection(it);
		return true;
	}
	IhaveResponse = response->stillSend;
	if (!IhaveResponse){
		if (response->isCGI)
			Cgi::CgiUnlink(response->cgiInfo);
		delete  response;
		response = NULL;
		switchEvent(getFd(), POLLIN);
	}
	return true;
}

bool Client::CgiRequest(){
	switchEvent(this->pfd.fd, POLLOUT);
	if (response->CgiResponse(*req)){
		CGIFinish = true;
		delete req;
		IhaveResponse = response->stillSend;		
		if (!IhaveResponse){
			switchEvent(this->pfd.fd, POLLIN);
			response->isCGI = false;
			Cgi::CgiUnlink(response->cgiInfo);
			delete response;
			response = NULL;
		}
		return true;
	}
	return false;	
} 



bool Client::NewRequest(ITT_CLIENT it, Server &ser){
	Request *req;

	this->keepAlive = true;
	if (!ReadRequest()){
		this->keepAlive = false;
		this->IhaveResponse = false;
		return true;
	}
	if (!isRequestAvailable())
		return true;
	switchEvent(this->pfd.fd, POLLOUT);
	req = getRequest();
	reqBuff.clear();
	if (req->getType() < 0)
	{
		this->response = new Response(this->pfd.fd);
		response->errorPage = ser.getErrorPages();
		response->setCode(req->getType());
		delete req;
		return false;
	}
	response = GenerateResponse::generateResponse(ser, *req, this->pfd.fd);
	IhaveCGI = response->isCGI;
	if (response->isCGI){
		this->req = req;
		return true;
	}
	if (!response->sendResponse()){
		delete req;
		return false;
	}
	IhaveResponse = response->stillSend;
	if (!IhaveResponse){
		delete  response;
		response = NULL;
		switchEvent(this->pfd.fd, POLLIN);
	}
	this->keepAlive = req->getConnection();
	delete req;
	return true;
}
