#pragma once

#include "../../include/includes.hpp"
#include "../../HttpElement/include/Server.hpp"
#include "../../HttpElement/include/Request.hpp"
#include "Response.hpp"

class GenerateResponse {
	
	public:
		static Response *generateResponse(Server &ser, Request &req, int fd);
		static std::string generateMsg(int Code);
		static std::string generateHeaderAndSt(Response &res, bool keepAlive);
};
