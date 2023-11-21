#include "../include/ErrorResponse.hpp"
#include <string>

void ErrorResponse::setErrorCode(int code)	{errorCode = code;}

void	ErrorResponse::setErrorMsg(std::string msg)	{errorMsg = msg;}

void	ErrorResponse::setErrorBody(std::string body)	{errorBody = body;}


int ErrorResponse::getErrorCode()	{return errorCode;}

std::string ErrorResponse::getErrorMsg()	{return errorMsg;}

std::string ErrorResponse::getErrorBody()	{return errorBody;}

std::string convertCode(int code){
	std::string codeStr;
	std::stringstream ss;
	ss << code;
	codeStr = ss.str();
	return codeStr;
}

std::string ErrorResponse::getErrorPage(bool KeepAlive){
	errorCodeStr = convertCode(errorCode);	
	std::string connection = "Connection: Close\r\n";
	if (KeepAlive)
		connection = "Connection: keep-alive\r\nKeep-Alive: timeout=" + convertCode(TIME_OUT) + "\r\n";

	errorPage = "HTTP/1.1 " + errorCodeStr + " " + errorMsg + "\r\n";
	errorPage += "Server: webserver\r\n";
	errorPage += "Content-Type: text/html\r\n";
	errorPage += "Content-Length: " + convertCode(errorBody.length()) + "\r\n";
	errorPage += connection + "\r\n";
	errorPage += errorBody;
	return 	errorPage;
}
