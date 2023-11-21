
#include "../include/GenerateResponse.hpp"
#include "../include/AllMethod.hpp"
#include "../include/Response.hpp"



Response *GenerateResponse::generateResponse(Server &ser, Request &req, int fd){
	Response *res = new Response(fd);

	res->errorPage = ser.getErrorPages();
	ResponseHandler Handler(ser, req, *res);

	
	res->setMsg(generateMsg(res->getCode()));
	res->setHeaderAndStart(generateHeaderAndSt(*res, req.getConnection()));

	return res;
}

void extractStatus(Response &res, std::string header){
	std::vector<std::string> cc = splitStream(header);
	res.setCode(std::atoi(cc[1].c_str()));
	res.setMsg(cc[2]);
}

std::string GenerateResponse::generateHeaderAndSt(Response &res, bool keepAlive){
	std::string str;
	std::string headersTmp = "";
	std::string connection = "Connection: close";
	bool content = false;

	str = res.getVersion() + " " + convertCode(res.getCode()) + " " + res.getMsg() + "\r\n";
	std::vector<std::string> headers = res.getHeaders();
	if (keepAlive)
		connection = "Connection: keep-alive\r\nKeep-Alive: timeout=" + convertCode(TIME_OUT);
	headers.push_back(connection);
	for (unsigned int i = 0;i < headers.size();i++){
		headersTmp += headers[i] + "\r\n";
		if (headers[i].find("Status: ") != headers[i].npos){
			extractStatus(res, headers[i]);
			str = res.getVersion() + " " + convertCode(res.getCode()) + " " + res.getMsg() + "\r\n";
		}
		if (headers[i].find("Content-Length: ") != headers[i].npos)
			content = true;
	}
	if (content == false && res.isCGI == false)
		res.setHeadr("Content-Length: 0\r\n");
	str += headersTmp + "\r\n";
	return str;
}


std::string GenerateResponse::generateMsg(int Code)
{
	if (Code == 100) return "Continue";
	else if (Code == 101) return "Switching Protocols";
	else if (Code == 102) return "Processing";
	else if (Code == 103) return "Early Hints";

	else if (Code == 200) return "OK";
	else if (Code == 201) return "Created";
	else if (Code == 202) return "Accepted";
	else if (Code == 203) return "Non-Authoritative Information";
	else if (Code == 204) return "No Content";
	else if (Code == 205) return "Reset Content";
	else if (Code == 206) return "Partial Content";
	else if (Code == 207) return "Multi-Status";
	else if (Code == 208) return "Already Reported";
	else if (Code == 226) return "IM Used";

	else if (Code == 300) return "Multiple Choices";
	else if (Code == 301) return "Moved Permanently";
	else if (Code == 302) return "Found";
	else if (Code == 303) return "See Other";
	else if (Code == 304) return "Not Modified";
	else if (Code == 305) return "Use Proxy";
	else if (Code == 307) return "Temporary Redirect";
	else if (Code == 308) return "Permanent Redirect";
	else if (Code == 405) return "Method Not Allowed";
	else if (Code == 403) return "Forbidden";
	else return "Not Found";
}
