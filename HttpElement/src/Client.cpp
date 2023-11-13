
#include "../include/Client.hpp"
#include "../include/Global.hpp"
#include "../include/Server.hpp"
#include "../../Response/include/GenerateResponse.hpp"
#include "../../Uploader/include/Upload.hpp"

#define N_READ 50000

#include "../../Utils/include/Logger.hpp"
Client::Client(int bodySize, int fd) : reqBuff(bodySize){

	lastTime = std::time(NULL);
	IhaveResponse = false;
	IhaveUpload = false;
	IhaveCGI = false;
	keepAlive = true;
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
	Logger::fastLog(Logger::INFO, "./Log/" + id,  "------------------- Start buffer request -------------------");
	Logger::fastLog(Logger::INFO, "./Log/" + id,  buffer);
	Logger::fastLog(Logger::INFO, "./Log/" + id,  "------------------ End buffer request --------------------");
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
		Logger::fastLog(Logger::ERROR, "./Log/" + id,  " error while handling old requeset");
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
		Logger::fastLog(Logger::INFO, "./Log/" + id,  "client switch event to POLLOUT");
	if (response->CgiResponse(this->keepAlive)){
		Logger::fastLog(Logger::INFO, "./Log/" + id,  "Cgi Finished");
		CGIFinish = true;
		IhaveResponse = response->stillSend;
		if (!IhaveResponse){
			Logger::fastLog(Logger::INFO, "./Log/" + id,  "Cgi response Send finish");
			switchEvent(this->fd, POLLIN);
			Logger::fastLog(Logger::INFO, "./Log/" + id,  "client switch event to POLLIN");
			response->isCGI = false;
			Cgi::CgiUnlink(response->cgiInfo);
		Logger::fastLog(Logger::INFO, "./Log/" + id,  "client unlink cgi files");
			delete response;
			response = NULL;
		}
		return true;
	}
	if (response->errrCgi){
			Logger::fastLog(Logger::INFO, "./Log/" + id,  "Cgi Error");
		IhaveResponse = false;
		response->sendErrorResponse(getFd());
		ser.closeConnection(it);
		return true;
	}
	Logger::fastLog(Logger::INFO, "./Log/" + id,  "Cgi still waiting");
	return false;
}

void Client::ClientUpload(Server &ser){

		Upload reminder(ser, *response);
		IhaveUpload = response->iHaveUpload;
		if (!IhaveUpload)
		{
			response->setMsg(GenerateResponse::generateMsg(response->getCode()));;
			response->setHeaderAndStart(GenerateResponse::generateHeaderAndSt(*response, keepAlive));
			response->sendResponse();
			IhaveResponse = false;
			switchEvent(this->fd, POLLIN);
		}
}

#include "../../Utils/include/Logger.hpp"

bool Client::NewRequest(Server &ser){
	Request *req;

	this->keepAlive = true;
	if (!ReadRequest())
	{ //TODO : send 500 if read fail
		std::cout << "Read Fail" << std::endl;
		this->keepAlive = false;
		this->IhaveResponse = false;
		return true;
	}
	if (!isRequestAvailable())
	{
		Logger::fastLog(Logger::INFO, "./Log/" + id,  "request not available");
		return true;
	}

	Logger::fastLog(Logger::INFO, "./Log/" + id,  "request is available");
	switchEvent(this->fd, POLLOUT);
	Logger::fastLog(Logger::INFO, "./Log/" + id,  "switch event to POLLOUT");
	req = getRequest();
	Logger::fastLog(Logger::INFO, "./Log/" + id, ("get request path: " + req->getPath()));
	reqBuff.clear();
	if (req->getType() < 0)
	{
		Logger::fastLog(Logger::INFO, "./Log/" + id,  "Bad request");
		this->response = new Response(this->fd);
		response->errorPage = ser.getErrorPages();
		response->setCode(req->getErrorCode());
		delete req;
		return false;
	}
	Server &server = Global::FindServer(req->getHeaders(),  ser);
	response = GenerateResponse::generateResponse(server, *req, this->fd);

	Logger::fastLog(Logger::INFO, "./Log/" + id,  "response generated");

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
		Logger::fastLog(Logger::INFO, "./Log/" + id,  "client switch event to POLLIN");
	}
	return true;
}
