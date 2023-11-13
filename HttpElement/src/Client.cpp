
#include "../include/Client.hpp"
#include "../include/Global.hpp"
#include "../include/Server.hpp"
#include "../../Response/include/GenerateResponse.hpp"
#include "../../Uploader/include/Upload.hpp"

#define N_READ 50000

Client::Client(int bodySize, int fd) : reqBuff(bodySize){

	lastTime = std::time(NULL);
	IhaveResponse = false;
	IhaveUpload = false;
	IhaveCGI = false;
	this->response = NULL;
	this->fd = fd;
}

Client::~Client(){
	delete this->response;
}

int Client::getFd(){return fd; }

bool Client::ReadRequest(){ //TODO : send 500 if read fail

	char *buffer;
	buffer = new (std::nothrow) char[N_READ];
	if (!buffer){
		std::cerr << "Fail to allocate for read" << std::endl;
		return false;
	}
	memset(buffer, 0, N_READ);
	status = recv(this->fd, buffer, N_READ, 0);
	if (status == -1 || status == 0)
	{
		delete  [] buffer;
		return false;
	}
	reqBuff.insertBuffer(buffer, status);
	delete [] buffer;
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
	Global::switchEvent(fd, Flag);
}

bool Client::OldRequest(ITT_CLIENT it, Server &ser){

	if (!response->ReminderResponse() && !response->errorInSend){
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

bool Client::CgiRequest(ITT_CLIENT it, Server &ser){
	switchEvent(this->fd, POLLOUT);
	if (response->CgiResponse(this->keepAlive)){
		CGIFinish = true;
		IhaveResponse = response->stillSend;
		if (!IhaveResponse){
			switchEvent(this->fd, POLLIN);
			response->isCGI = false;
			Cgi::CgiUnlink(response->cgiInfo);
			delete response;
			response = NULL;
		}
		return true;
	}
	if (response->errrCgi){
		IhaveResponse = false;
		response->sendErrorResponse(getFd());
		ser.closeConnection(it);
		return true;
	}
	return false;
}

void Client::ClientUpload(Server &ser){

		Upload reminder(ser, *response);
		IhaveUpload = response->iHaveUpload;
		if (!IhaveUpload)
		{
			std::cout << "Reminder upload" << std::endl;
			response->setMsg(GenerateResponse::generateMsg(response->getCode()));;
			response->setHeaderAndStart(GenerateResponse::generateHeaderAndSt(*response, keepAlive));
			response->sendResponse();
			IhaveResponse = false;
			// switchEvent(this->fd, POLLIN);
		}
}


bool Client::NewRequest(Server &ser){
	Request *req;

	this->keepAlive = true;
	if (!ReadRequest()){ //TODO : send 500 if read fail
		this->keepAlive = false;
		this->IhaveResponse = false;
		return true;
	}
	if (!isRequestAvailable())
		return true;
	switchEvent(this->fd, POLLOUT);
	req = getRequest();
	reqBuff.clear();
	if (req->getType() < 0)
	{
		this->response = new Response(this->fd);
		response->errorPage = ser.getErrorPages();
		response->setCode(req->getErrorCode());
		delete req;
		return false;
	}
	Server &server = Global::FindServer(req->getHeaders(),  ser);
	response = GenerateResponse::generateResponse(server, *req, this->fd);
	this->keepAlive = req->getConnection();
	delete req;
	IhaveCGI = response->isCGI;
	IhaveUpload = response->iHaveUpload;
	IhaveResponse = response->stillSend;
	if (response->isCGI || response->iHaveUpload)
		return true;
	if (!response->sendResponse())
		return false;
	IhaveResponse = response->stillSend;
	if (!IhaveResponse){
		delete  response;
		response = NULL;
		switchEvent(this->fd, POLLIN);
	}
	return true;
}
