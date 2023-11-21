
#include "../include/Client.hpp"
#include "../include/Global.hpp"
#include "../include/Server.hpp"
#include "../../Response/include/GenerateResponse.hpp"
#include "../../Uploader/include/Upload.hpp"
#include "../../Utils/include/Logger.hpp"

#define N_READ 50000

Client::Client(size_t bodySize, int fd) : reqBuff(bodySize){

	lastTime = std::time(NULL);
	IhaveResponse = false;
	IhaveUpload = false;
	IhaveCGI = false;
	CGIFinish = false;
	keepAlive = true;
	this->response = NULL;
	this->fd = fd;
}

Client::~Client(){
	clearClient();
	delete this->response;
}

int Client::getFd(){return fd; }

std::time_t Client::getLastTime(){ return lastTime;}

void Client::setLastTime(std::time_t tm){ this->lastTime = tm;}

void Client::switchEvent(int fd, int Flag) {Global::switchEvent(fd, Flag);}


bool Client::isRequestAvailable(){
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

void Client::clearClient(){

	if (!response)
		return;
	if (this->IhaveCGI && this->CGIFinish)
		Cgi::CgiUnlink(response->cgiInfo);
	else if (this->IhaveCGI && !this->CGIFinish){
		Cgi::KillCgi(response->cgiInfo);
		Cgi::CgiUnlink(response->cgiInfo);
	}
	if (this->IhaveUpload)
		unlink(response->_source_file.c_str());
}

bool Client::OldRequest(){

	if (!response->ReminderResponse())
		return false;
	IhaveResponse = response->stillSend;
	if (!IhaveResponse){
		if (response->isCGI)
			Cgi::CgiUnlink(response->cgiInfo);
		resetClient();
	}
	return true;
}

bool Client::CgiRequest(){

	switchEvent(this->fd, POLLOUT);
	if (response->CgiResponse(this->keepAlive)){
		CGIFinish = true;
		IhaveResponse = response->stillSend;
		if (!IhaveResponse){
			Cgi::CgiUnlink(response->cgiInfo);
			this->resetClient();
		}
		return true;
	}
	if (response->errrCgi){
		Cgi::CgiUnlink(response->cgiInfo);
		response->sendErrorResponse(getFd(), this->keepAlive);
		resetClient();
		return true;
	}
	return false;
}

void Client::ClientUpload(Server &ser){

		Upload reminder(ser, *response);
		IhaveUpload = response->iHaveUpload;
		if (!IhaveUpload)
		{
			response->setMsg(GenerateResponse::generateMsg(response->getCode()));
			response->setHeaderAndStart(GenerateResponse::generateHeaderAndSt(*response, keepAlive));
			response->sendResponse();
			resetClient();
		}
}

void Client::resetClient(){

	delete this->response;
	this->response = NULL;
	IhaveResponse = false;
	IhaveUpload = false;
	IhaveCGI = false;
	CGIFinish = false;
	switchEvent(this->fd, POLLIN);
}

bool Client::ReadRequest(){

	char buffer[N_READ];
	memset(buffer, 0, N_READ);
	ssize_t status = recv(this->fd, buffer, N_READ, 0);
	if (status == 0 || status == -1){
		std::cerr << "Error while read request from client" << std::endl;
		return false;
	}
	try{
		reqBuff.insertBuffer(buffer, status);
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		reqBuff._status = 500;
		return true;
	}
	return true;
}

bool Client::NewRequest(Server &ser){
	Request *req;

	this->keepAlive = true;
	if (!ReadRequest()){
		this->IhaveResponse = false;
		this->keepAlive = false;
		return true;
	}
	if (!isRequestAvailable()){
		return true;
	}
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
	CGIFinish = false;
	if (response->isCGI || response->iHaveUpload)
		return true;
	if (!response->sendResponse())
		return false;
	IhaveResponse = response->stillSend;
	if (!IhaveResponse)
		this->resetClient();

	return true;
}
