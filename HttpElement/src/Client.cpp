
#include "../include/Client.hpp"
#include "../include/Global.hpp"
#include "../include/Server.hpp"
#include "../../Response/include/GenerateResponse.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <sys/_types/_size_t.h>
#include <vector>

#define N_READ 50000

Client::Client(int bodySize, int fd) : reqBuff(bodySize){

	lastTime = std::time(NULL);
	IhaveResponse = false;
	IhaveUpload = false;
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

	if (!response->ReminderResponse() && !response->errorInSend){
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
		response->setCode(req->getErrorCode());
		delete req;
		return false;
	}
	Server &server = Global::FindServer(req->getHeaders(),  ser);
	response = GenerateResponse::generateResponse(server, *req, this->pfd.fd);
	IhaveCGI = response->isCGI;
	if (response->isCGI){
		this->req = req;
		return true;
	}
	if (!response->sendResponse()){
		delete req;
		return false;
	}
	IhaveUpload = response->iHaveUpload;
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
