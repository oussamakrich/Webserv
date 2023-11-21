#include "../include/GenerateError.hpp"
#include <string>

ErrorResponse	GenerateError::generateError(int code, VECT_ERRORPIR errorPage){
	ErrorResponse error;

	error.setErrorCode(code);
	error.setErrorMsg(generateMsg(code));
	error.setErrorBody(generateBody(code, errorPage));
	return error;
}

std::string	GenerateError::generateMsg(int code){
	if (code == 400) return "Bad Request";
	if (code == 401) return "Unauthorized";
	if (code == 402) return "Payment Required";
	if (code == 403) return "Forbidden";
	if (code == 404) return "Not Found";
	if (code == 405) return "Method Not Allowed";
	if (code == 406) return "Not Acceptable";
	if (code == 407) return "Proxy Authentication Required";
	if (code == 408) return "Request Timeout";
	if (code == 409) return "Conflict";
	if (code == 410) return "Gone";
	if (code == 411) return "Length Required";
	if (code == 412) return "Precondition Failed";
	if (code == 413) return "Content Too Large";
	if (code == 414) return "URI Too Long";
	if (code == 415) return "Unsupported Media Type";
	if (code == 416) return "Range Not Satisfiable";
	if (code == 417) return "Expectation Failed";
	if (code == 500) return "Internal Server Error";
	if (code == 501) return "Not Implemented";
	if (code == 502) return "Bad Gateway";
	if (code == 503) return "Service Unavailable";
	if (code == 504) return "Gateway Timeout";
	if (code == 505) return "HTTP Version Not Supported";
	if (code == 507) return "Insufficient Storage";
	else return "Undefined Error";
}

std::string findPage(VECT_ERRORPIR errorPage, int code){
	ITT_ERRORPIR it = errorPage.begin();
	for (; it != errorPage.end(); it++){
		if (it->first == code)
			return it->second;
	}
	return "";
}

std::string simpleBody(int code){
	std::string page;
	std::stringstream ss;
	ss << code;
	page = "<html><head ><title>" + ss.str() + " " + GenerateError::generateMsg(code) + "</title></head>";
	page += "<body style='color: #F99417; background-color : #363062;'>";
	page += "<center><h1>" + ss.str() + " " + GenerateError::generateMsg(code) + "</h1></center>";
	page += "<hr style='color:#F99417'><center><h4>Hustler Server v0.1</h4></center></body></html>";
	return page;
}


std::string	GenerateError::generateBody(int code, VECT_ERRORPIR errorPage){
	std::string body;
	std::string page;

	page = findPage(errorPage, code);
	if (page.empty())
		return simpleBody(code);
	std::ifstream file(page.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: errorPage couldn't open " << std::endl;
		return simpleBody(code);
	}
	std::string line;
	while (getline(file, line))
		body += line;
	file.close();
	return body;
}
