#pragma once

#include "../../include/includes.hpp"
#include "ErrorResponse.hpp"
#include "../../HttpElement/include/Server.hpp"

class GenerateError{
	public:
		static ErrorResponse generateError(int code, Server &serv);
		static std::string	generateMsg(int code);
		static std::string	generateBody(int code, VECT_ERRORPIR errorPage);
};
