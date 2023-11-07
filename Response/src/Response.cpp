#include "../include/Response.hpp"
#include "../../ErrorResponse/include/ErrorResponse.hpp"
#include "../../ErrorResponse/include/GenerateError.hpp"
#include "../../HttpElement/include/Server.hpp"
#include "../include/GenerateResponse.hpp"
#include "../../include/includes.hpp"
#include <sys/_types/_size_t.h>


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

bool Response::CgiHeaders(Request &req){
	path = cgiInfo.output;
	size_t sizeOfFile;
	isFile(path, sizeOfFile);

	std::ifstream file(path);	
	if (!file.is_open()){
		setCode(500);	
		return false;
	}
	std::string header;
	size_t pos;
	size_t headerPos = 0;
	while (!file.eof()){
		std::getline(file, header);	
		if (header.empty()) 
			break;
		pos = header.find('\r');
		if (pos != header.npos)
			header.erase(pos);
		if (header.empty())
			break;
		headers.push_back(header);		
	}
	headerPos = file.tellg();
	this->msg = "OK";
	this->code = 200;
	setHeadr("Content-Length: " + convertCode(sizeOfFile - headerPos));
	this->HeaderAndStart = GenerateResponse::generateHeaderAndSt(*this, req);	
	buffer = new char[R_READ];
	file.read(buffer, R_READ);
	bufferSize = file.gcount();
	this->pos = bufferSize + headerPos;
	stillSend = true;
	if (file.eof())
		stillSend = false;
	file.close();
	return true;
}

bool Response::CgiResponse(Request &req){

	int status;
	if (Cgi::isFinished(cgiInfo, status)){
		CgiHeaders(req);
		sendResponse();
		return true;
	}
	return false;
}

bool Response::sendResponse(){

	errorInSend = false;
	if (this->code >= 400)
		return false;
	const char *resp = this->strjoin(HeaderAndStart.c_str(), buffer, HeaderAndStart.size(), bufferSize);
	buffer = NULL;
	int ret = send(fd, resp, HeaderAndStart.size() + bufferSize, 0);
	delete  resp;
	if (ret == -1 || ret == 0){
	 std::cout << "Error send" << std::endl;
		errorInSend = true;
		return false;
	}
	return true;
}

bool Response::ReminderResponse(){
	errorInSend = false;
	std::ifstream file(path.c_str());
	if (!file.is_open()){
		setCode(500);
		return false;
	}
	char *buffer = new char[R_READ];
	file.seekg(pos);
	file.read(buffer, R_READ);
	setBuffer(buffer, file.gcount());
	pos += file.gcount();
	stillSend = true;
	if (file.eof())
		stillSend = false;
	file.close();
	int ret = send(fd, buffer, bufferSize, 0);
	delete [] buffer;
	if (ret == -1 || ret == 0) {
		std::cout << "Error send : "<< std::endl;
		errorInSend = true;
		return false;
	}
	return true;
}
