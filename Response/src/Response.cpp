#include "../include/Response.hpp"
#include "../../ErrorResponse/include/ErrorResponse.hpp"
#include "../../ErrorResponse/include/GenerateError.hpp"
#include "../../HttpElement/include/Server.hpp"
#include "../include/GenerateResponse.hpp"
#include "../../include/includes.hpp"
#include "../../Utils/include/Logger.hpp"
#include "../../HttpElement/include/Global.hpp"

Response::Response(int fd){
	this->_seek_pos = 0;
	this->code = 0;
	this->pos = 0;
	this->fd = fd;
	this->version = "HTTP/1.1";
	this->msg = "";
	this->iHaveUpload = false;
	this->headers = std::vector<std::string>();
	this->buffer = NULL;
	this->bufferSize = 0;
	this->errrCgi = false;
	this->iHaveUpload = false;// false
	this->stillSend = false;// sttll send the reminder
	this->isCGI = false;
	this->redirection = false;
}

Response::~Response(){
	delete [] this->buffer;
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

void Response::setHeaderAndStart(std::string header){this->HeaderAndStart = header;}

void Response::sendErrorResponse(int fd, bool keepAlive){
		stillSend = false;
		ErrorResponse err = GenerateError::generateError(this->code, this->errorPage);
		std::string error =  err.getErrorPage(keepAlive);
		int ret = send(fd, error.c_str(), error.size(), 0);
		if (ret == 0 || ret == -1)
			close(fd);

}

bool parseHeader(std::string header){
	std::string key, value;
	size_t pos;

	pos = header.find(':');
	if (pos == header.npos || pos == 0 || pos == header.size() - 1)
		return false;
	key = header.substr(0, pos);
	value = header.substr(pos + 1);
	if (!ParsRequest::isValidKey(key))
		return false;
	for (size_t i = 0;i < value.size(); i++){
		if (!std::isprint(value[i]))
			return false;
	}
	return true;
}

void Response::CgiHeaders(bool keepAlive, std::ifstream &file, size_t sizeOfFile){
	std::string header;
	size_t pos, headerPos = 0;
	bool thereIsLenght = false;

	while (!file.eof()){
		std::getline(file, header);
		if (header.empty())
			break;
		pos = header.find('\r');
		if (pos != header.npos)
			header.erase(pos);
		if (header.empty())
			break;
		if (!parseHeader(header)){
			this->code = 502;
			return;
		}
		headers.push_back(header);
	}
	headerPos = file.tellg();
	this->msg = "OK";
	this->code = 200;
	for (size_t i =0; i < headers.size(); i++){		
		if (headers[i].find("Content-Length") != std::string::npos)
			thereIsLenght = true;
	}
	if (file.eof() && !thereIsLenght)
		setHeadr("Content-Length: 0");
	else if (!thereIsLenght)
			setHeadr("Content-Length: " + convertCode(sizeOfFile - headerPos));
	this->HeaderAndStart = GenerateResponse::generateHeaderAndSt(*this, keepAlive);
	this->pos = headerPos;
}

bool Response::CgiRead(bool keepAlive){
	path = cgiInfo.output;
	size_t sizeOfFile;
	isFile(path, sizeOfFile);

	std::ifstream file(path);
	if (!file.is_open()){
		setCode(500);
		return false;
	}
	CgiHeaders(keepAlive, file, sizeOfFile);
	if (file.eof()){
		file.close();
		stillSend = false;
		return true;
	}
	buffer = new(std::nothrow) char[R_READ];
	if (!buffer){
		setCode(500);
		return false;
	}
	std::memset(buffer, 0, R_READ);
	file.read(buffer, R_READ);
	bufferSize = file.gcount();
	this->pos += bufferSize;
	stillSend = !file.eof();
	file.close();
	return true;
}

bool Response::CgiResponse(bool keepAlive){

	int status;
	errrCgi = false;
	if (Cgi::isFinished(cgiInfo, status)){
		if (status != 0){
			setCode(502);
			errrCgi	= true;
			return false;
		}
		if (!CgiRead(keepAlive) || !sendResponse()){
			errrCgi = true;
			return false;
		}
	}
	if (Cgi::isTimeOut(cgiInfo)){
		setCode(504);
		Cgi::KillCgi(cgiInfo);
		errrCgi	= true;
		return false;
	}
	return false;
}

bool Response::sendResponse(){
	if (this->code >= 400 && this->redirection == false)
		return false;
	const char *resp = Responsejoin(HeaderAndStart.c_str(), buffer, HeaderAndStart.size(), bufferSize);
	buffer = NULL;
	if (!resp){
		this->code = 500;	
		return false;
	}
	int ret = send(fd, resp, HeaderAndStart.size() + bufferSize, 0);
	delete []  resp;
	if (ret == 0 || ret == -1)
		return true;				
	if (ret - HeaderAndStart.size() != bufferSize)
		pos -= ret - HeaderAndStart.size();
	this->redirection = false;
	return true;
}

bool Response::ReminderResponse() {
	
	std::ifstream file(path.c_str());
	if (!file.is_open())
		return false;
	buffer = new char[R_READ];
	memset(buffer, 0, R_READ);
	file.seekg(pos);
	file.read(buffer, R_READ);
	bufferSize = file.gcount();
	pos += file.gcount();
	stillSend = !file.eof();
	file.close();
	int ret = send(fd, buffer, bufferSize, 0);
	delete [] buffer;
	buffer = NULL;
	if (ret == 0 || ret == -1)
		return false;

	if (file.gcount() != ret)
		pos -= file.gcount() - ret;
	this->redirection = false;
	return true;
}
