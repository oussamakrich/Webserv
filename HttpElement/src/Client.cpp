
#include "../include/Client.hpp"
#include "../include/Global.hpp"
#include "../include/Server.hpp"
#include "../../Response/include/GenerateResponse.hpp"

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
	if (!buffer)
		return false;
	memset(buffer, 0, N_READ);
	status = recv(this->fd, buffer, N_READ, 0);
	if (status == -1 || status == 0)
	{
		delete  buffer;
		return false;
	}
	reqBuff.insertBuffer(buffer, status);
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
	Global::switchEvent(fd, Flag);
}

bool Client::OldRequest(ITT_CLIENT it, Server &ser){

	if (!response->ReminderResponse() && !response->errorInSend){
		response->sendErrorResponse(getFd());
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

Server &Global::FindServer(const MAP_STRING &headers, Server &ser){

	std::string value;
	//TODO : first check if host is only alpha

	value = headers.at("Host");
	if (value.empty())
		return ser;
	stringstream ss(value);
	std::getline(ss, value, ':');

	std::vector<std::string>::iterator it;

	it = std::find(serverNames.begin(), serverNames.end(), value);
	if (it == serverNames.end() || value == ser.getServerName())
		return ser;

	size_t size = servers.size();
	for (size_t i = 0;i < size; i++){
		if (servers[i]->getServerName() == value && servers[i]->getPort() == ser.getPort() && servers[i]->getHost() == ser.getHost())
			return *servers[i];
	}
	return ser;
}


bool Client::NewRequest(Server &ser){
	Request *req;

	this->keepAlive = true;
	if (!ReadRequest()){
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
	if (response->isCGI || response->iHaveUpload)
		return true;
	if (!response->sendResponse())
		return false;
	IhaveUpload = response->iHaveUpload;
	IhaveResponse = response->stillSend;
	if (!IhaveResponse){
		delete  response;
		response = NULL;
		switchEvent(this->fd, POLLIN);
	}
	return true;
}
