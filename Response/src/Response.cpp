#include "../include/Response.hpp"
#include "../../ErrorResponse/include/ErrorResponse.hpp"
#include "../../ErrorResponse/include/GenerateError.hpp"
#include "../../HttpElement/include/Server.hpp"
#include "../include/GenerateResponse.hpp"
#include "../../include/includes.hpp"


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
}

Response::~Response(){
	if (this->buffer != NULL)
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

void Response::sendErrorResponse(int fd){
		stillSend = false;
		ErrorResponse err = GenerateError::generateError(this->code, this->errorPage);
		std::string error =  err.getErrorPage();
		send(fd, error.c_str(), error.size(), 0);
}



void Response::CgiHeaders(bool keepAlive, std::ifstream &file, size_t sizeOfFile){
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
	if (file.eof())
		setHeadr("Content-Length: 0");
	else
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
	buffer = new char[R_READ];
	file.read(buffer, R_READ);
	bufferSize = file.gcount();
	this->pos += bufferSize;
	stillSend = true;
	if (file.eof())
		stillSend = false;
	file.close();
	return true;
}

bool Response::CgiResponse(bool keepAlive){

	int status;
	if (Cgi::isFinished(cgiInfo, status)){
		if (status != 0){
			setCode(500);
			stillSend = false;
			errrCgi	= true;
			Cgi::CgiUnlink(cgiInfo);
			return false;
		}
		CgiRead(keepAlive);
		sendResponse();
		return true;
	}
	if (Cgi::isTimeOut(cgiInfo)){
		setCode(504);
		Cgi::CgiUnlink(cgiInfo);
		Cgi::KillCgi(cgiInfo);
		stillSend = false;
		errrCgi	= true;
		return false;
	}
	return false;
}

bool Response::sendResponse(){

	errorInSend = false;
	if (this->code >= 400)
		return false;
	const char *resp = Responsejoin(HeaderAndStart.c_str(), buffer, HeaderAndStart.size(), bufferSize);
	buffer = NULL;
	int ret = send(fd, resp, HeaderAndStart.size() + bufferSize, 0);
	delete []  resp;
	if (ret == -1 || ret == 0)
	{
	 std::cout << "Error send" << std::endl;
		errorInSend = true;
		return false;
	}
	return true;
}

bool Response::ReminderResponse(){
	errorInSend = false;
	std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open()){
		setCode(500);
		return false;
	}
	buffer = new char[R_READ];
	file.seekg(pos);
	file.read(buffer, R_READ);
	bufferSize =  file.gcount();
	pos += file.gcount();
	stillSend = true;
	if (file.eof())
		stillSend = false;
	file.close();
	int ret = send(fd, buffer, bufferSize, 0);
	if (file.gcount() != ret)
	{
		std::cout << "ret: " << ret << " count: " << file.gcount() << std::endl;
		pos -= file.gcount() - ret;
	}
	delete [] buffer;
	buffer = NULL;
	if (ret == -1 || ret == 0)
	{
		std::cout << "Error send reminder : " << ret << std::endl;
		errorInSend = true;
		return false;
	}
	return true;
}
