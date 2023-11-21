#include "../include/ParsRequest.hpp"
#include <iostream>



Request *ParsRequest::Pars(RequestBuffer &reqBuff)
{
	int type = reqBuff.getLevel() > 1 ? Request::INVALID_REQUEST : Request::GET;
	Request *req = new(std::nothrow) Request(type);
	if (req == NULL) return NULL;
	req->setErrorCode(reqBuff.getLevel());
	if (req->getType() == Request::INVALID_REQUEST) return req;
	try {
		std::string headers = reqBuff.getHeaders();
		std::vector<std::string> header_vect = split(headers, '\n');
		ParsFirstLine(*req, reqBuff);
		for (size_t i = 0; i < header_vect.size(); i++)
		{
			if (ParsHeaders(*req, header_vect[i]) == false)
			{
				req->setType(Request::INVALID_REQUEST);
				break;
			}
		}
		req->setBody(reqBuff.getBody());

		return req;
	}catch(std::exception &ex)
	{
		req->setType(Request::INVALID_REQUEST);
		req->setErrorCode(500);
		return req;
	}

}

 void  ParsRequest::ParsFirstLine(Request& req, RequestBuffer &reqBuff)
{
	try
	{
			req.setMethod(reqBuff.getMethod());
			req.setType(getMethodCode(req.getMethod()));
			std::string location = reqBuff.getURI();
			size_t query_pos = location.find('?');
			if (query_pos != std::string::npos) {
				req.setPath(location.substr(0, query_pos));
				req.setQuery(location.substr(query_pos + 1));
			} else
				req.setPath(location);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}


}

bool  ParsRequest::isValidKey(std::string key)
{
	try {
	std::string special_chars = "_.-";
	for(size_t i = 0; i < key.length(); i++)
	{
		if (std::isalnum(key.at(i)) == false  &&  special_chars.find(key.at(i)) == std::string::npos)
			return false;
	}
		return true;
	}
	catch(std::exception &ex)
	{

		return false;
	}
}

 bool  ParsRequest::ParsHeaders(Request& req, std::string& line)
{
	size_t pos = line.find(':');
	if (line.empty()) return true;
	if (pos == std::string::npos) return false;
	try
	{
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			value = trim(value);
			if (key.empty() || value.empty()) return true;
			if (isValidKey(key) == false) return false;


			if(key == "Content-Length")
			{
				if(isInteger(value) == false) return false;
				req.setContentLength(std::atoi(value.c_str()));
				return  req.insertHeader(key, value);
			}
			else if (key == "Transfer-Encoding")
				req.setTransferEncoding(value);
			else if (key == "Connection")
			{
				if (value == "keep-alive" || value == "close")
				{
					req.setConnection(value == "keep-alive");
					req.insertHeader(key, value);
				}
				else
				return false;

			}
			else
				return  req.insertHeader(key, value);
		}
		catch(std::exception &ex)
		{
				return false;
		}

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
