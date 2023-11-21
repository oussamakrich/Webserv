#pragma once

#include "../../include/includes.hpp"
#include "../../HttpElement/include/Server.hpp"

class ErrorResponse{

	private:
		int errorCode;
		std::string errorCodeStr;
		std::string errorMsg;
		std::string errorBody;
		std::string errorPage;

	public:
		void setErrorCode(int code);
		void setErrorMsg(std::string msg);
		void setErrorBody(std::string body);

	public:
		std::string getErrorMsg();
		std::string getErrorBody();
		int 				getErrorCode();
		std::string getErrorPage(bool KeepAlive);

};
