#include "../include/Response.hpp"
#include "../../ErrorResponse/include/ErrorResponse.hpp"
#include "../../ErrorResponse/include/GenerateError.hpp"
#include "../../HttpElement/include/Server.hpp"


Response::Response(int fd){
	this->code = 0;
	this->fd = fd;
	this->version = "HTTP/1.1";
	this->msg = "";
	this->headers = std::vector<std::string>();
	this->buffer = NULL;
	this->bufferSize = 0;
}

Response::~Response(){}

int Response::getCode(){ return this->code;}

std::string Response::getVersion(){ return this->version;}

std::string Response::getMsg(){ return this->msg;}

std::string *Response::getHeaderAndStart(){ return &this->HeaderAndStart;}

std::vector<std::string> Response::getHeaders(){ return this->headers;}

char *Response::getBuffer(){ return this->buffer;}

int Response::getBufferSize(){ return this->bufferSize;}

void Response::setCode(int code){ this->code = code;}

void Response::setVersion(std::string version){ this->version = version;}

void Response::setMsg(std::string msg){ this->msg = msg;}

void Response::setHeaders(std::vector<std::string> headers){ this->headers = headers;}

void Response::setBuffer(char *buffer, int size){ this->buffer = buffer; this->bufferSize = size;}

void Response::setHeadr(std::string header){ this->headers.push_back(header);}

void Response::setHeaderAndStart(std::string header){this->HeaderAndStart = header;}

void Response::sendErrorResponse(Server &ser ,int fd){
		stillSend = false;
		ErrorResponse err = GenerateError::generateError(this->code, this->errorPage);
		std::string error =  err.getErrorPage(ser);
		send(fd, error.c_str(), error.size(), 0);
}

bool Response::sendResponse(){

	if (this->code >= 400)
		return false;
	const char *resp = this->strjoin(HeaderAndStart.c_str(), buffer, HeaderAndStart.size(), bufferSize);
	buffer = NULL;
	int ret = send(fd, resp, HeaderAndStart.size() + bufferSize, 0);
	if (ret == -1) std::cout << "Error send" << std::endl;
	delete  resp;
	return true;
}

bool Response::ReminderResponse(){

	std::ifstream file(path.c_str());
	if (file.is_open()){
		char *buffer = new char[R_READ];
		file.seekg(pos);
		file.read(buffer, R_READ);
		setBuffer(buffer, file.gcount());
		pos += file.gcount();
		stillSend = true;
		if (file.eof())
			stillSend = false;
		file.close();
	}
	else{
		setCode(404);
		return false;
	}

	int ret = send(fd, buffer, bufferSize, 0);
	if (ret == -1) std::cout << "Error send" << std::endl; //FIX : error Response
	delete  buffer;
	return true;
}
