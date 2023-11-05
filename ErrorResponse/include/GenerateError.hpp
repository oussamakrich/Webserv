#pragma once

#include "../../include/includes.hpp"
#include "ErrorResponse.hpp"
#include "../../HttpElement/include/Server.hpp"

class GenerateError{
	public:
		static ErrorResponse generateError(int code, VECT_ERRORPIR errorPage);
		static std::string	generateMsg(int code);
		static std::string	generateBody(int code, VECT_ERRORPIR errorPage);
};
