
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

void Client::clearClient(){

	if (!response){
		Logger::fastLog(Logger::ERROR, "./Log/" + id,  " try to clear but i dont have response");
		return;
	}
	if (this->IhaveCGI && this->CGIFinish){
		Logger::fastLog(Logger::ERROR, "./Log/" + id,  " connection reset and cgi finish -> unlink files");
		Cgi::CgiUnlink(response->cgiInfo);
	}
	else if (this->IhaveCGI && !this->CGIFinish){
		Logger::fastLog(Logger::ERROR, "./Log/" + id,  " connection reset and cgi still wait -> KillCgi unlink files");
		Cgi::KillCgi(response->cgiInfo);
		Cgi::CgiUnlink(response->cgiInfo);
	}
	if (this->IhaveUpload) {
		Logger::fastLog(Logger::ERROR, "./Log/" + id,  " connection reset and I have upload -> unlink files");
		unlink(response->_source_file.c_str());
	}
}

bool Client::OldRequest(){

	if (!response->ReminderResponse() /*&& !response->errorInSend*/){
		// Logger::fastLog(Logger::ERROR, "./Log/" + id,  " error while handling old requeset");
		return false;
	}
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
		// Logger::fastLog(Logger::INFO, "./Log/" + id,  "client switch event to POLLOUT");
	if (response->CgiResponse(this->keepAlive)){
		// Logger::fastLog(Logger::INFO, "./Log/" + id,  "Cgi Finished");
		CGIFinish = true;
		IhaveResponse = response->stillSend;
		if (!IhaveResponse){
			// Logger::fastLog(Logger::INFO, "./Log/" + id,  "Cgi response Send finish");
			// Logger::fastLog(Logger::INFO, "./Log/" + id,  "client switch event to POLLIN");
			Cgi::CgiUnlink(response->cgiInfo);
			this->resetClient();
		// Logger::fastLog(Logger::INFO, "./Log/" + id,  "client unlink cgi files");
		}
		return true;
	}
	if (response->errrCgi){
		// Logger::fastLog(Logger::INFO, "./Log/" + id,  "Cgi Error");
		Cgi::CgiUnlink(response->cgiInfo);
		response->sendErrorResponse(getFd(), this->keepAlive);
		resetClient();
		return true;
	}
	// Logger::fastLog(Logger::INFO, "./Log/" + id,  "Cgi still waiting");
	return false;
}

void Client::ClientUpload(Server &ser){

		Upload reminder(ser, *response);
		IhaveUpload = response->iHaveUpload;
		if (!IhaveUpload)
		{
			response->setMsg(GenerateResponse::generateMsg(response->getCode()));
			response->setHeadr("Content-Length: 0");
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
	status = recv(this->fd, buffer, N_READ, 0);
	if (status == 0 || status == -1){
		std::cerr << "Error while read request from client" << std::endl;
		return false;
	}
	reqBuff.insertBuffer(buffer, status);
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
