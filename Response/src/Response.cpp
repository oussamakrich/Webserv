#include "../include/Response.hpp"


Response::Response(int fd){
	this->code = 0;
	this->fd = fd;
	this->version = "HTTP/1.1";
	this->msg = "";
	this->headers = std::vector<std::string>();
	this->buffer = NULL;
	this->bufferSize = 0;
}

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


void Response::sendResponse(){

	const char *resp = this->strjoin(HeaderAndStart.c_str(), buffer, HeaderAndStart.size(), bufferSize);

	int ret = send(fd, resp, HeaderAndStart.size() + bufferSize, 0);
	if (ret == -1) std::cout << "Error send" << std::endl;
	delete [] resp;
}
