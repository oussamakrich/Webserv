#include "../include/ParsRequest.hpp"



Request *ParsRequest::Pars(RequestBuffer &reqBuff)
{
	int type = reqBuff.getLevel() > 3 ? Request::INVALID_REQUEST : Request::GET;
	Request *req = new(std::nothrow) Request(type);
	if (req == NULL) return NULL;
	req->setErrorCode(reqBuff.getLevel());
	if (req->getType() == Request::INVALID_REQUEST) return req;
	std::string headers = replaceAll(reqBuff.getHeaders(), "\r\n", "\n");
	std::vector<std::string> header_vect = split(headers, '\n');
	ParsFirstLine(*req, reqBuff.getFirstLine());
	for (size_t i = 0; i < header_vect.size(); i++)
	 {
		if (ParsHeaders(*req, header_vect[i]) == false)
		{
			req->setType(Request::INVALID_REQUEST);
			break;
		}	
	}
	req->setBodyPath(reqBuff.getBody());
	return req;
}

 void  ParsRequest::ParsFirstLine(Request& req, std::string line) 
{
    int sp_pos = line.find(' ');
    req.setMethod(line.substr(0, sp_pos));
	req.setType(getMethodCode(req.getMethod()));
    int location_pos = line.find(' ', sp_pos + 1);
    std::string location = line.substr(sp_pos + 1, location_pos - sp_pos - 1);
    size_t query_pos = location.find('?');
    if (query_pos != std::string::npos) {
        req.setPath(location.substr(0, query_pos));
        req.setQuery(location.substr(query_pos + 1));
    } else 
        req.setPath(location);
}

bool  ParsRequest::isValidKey(std::string key)
{
	std::string special_chars = "_.-";
	for(size_t i = 0; i < key.length(); i++)
	{
		if (std::isalnum(key.at(i)) == false  &&  special_chars.find(key.at(i)) == std::string::npos)
			return false;
	}
	return true;
}

 bool  ParsRequest::ParsHeaders(Request& req, std::string& line) 
{
	size_t pos = line.find(':');
	if (pos == std::string::npos) return false;
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos + 1);
	if (isValidKey(key) == false) return false;
	if(key == "Content-Length") 
	{
		if(isInteger(value) == false) return false;
		req.setContentLength(std::atoi(value.c_str()));
	}
	else if (key == "Transfer-Encoding") 
		req.setTransferEncoding(value);
	else
		if (req.insertHeader(key, value) == false)
			return false; 
	return true;
}

//***************************** util  ? moved in other file ***************************************

std::string replaceAll(std::string str, const std::string s1, const std::string s2) {
    size_t pos = 0;
    while ((pos = str.find(s1, pos)) != std::string::npos) {
        str.replace(pos, s1.length(), s2);
        pos += s2.length();
    }
    return str;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int getMethodCode(std::string  method)
{
	if (method == "GET")	return 		Request::GET;
	if(method == "DELETE") 	return	 	Request::DELETE;	 
	if(method == "POST") 	return 		Request::POST;
	if(method == "PUT") 	return 		Request::NOT_SUPPORTED;
	if(method == "HEAD") 	return 		Request::NOT_SUPPORTED;
	if(method == "OPTIONS") return 		Request::NOT_SUPPORTED;
	if(method == "CONNECT") return 		Request::NOT_SUPPORTED;
	if(method == "TRACE") 	return 		Request::NOT_SUPPORTED;
	if(method == "PATCH") 	return 		Request::NOT_SUPPORTED;
	return								Request::INVALID_REQUEST;


}
