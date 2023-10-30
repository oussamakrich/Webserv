#include "../include/Request.hpp"


Request::Request(int type)
{
    type 			= type;
	error_code 		= 400;
	content_length 	= -1;
	method			= "GET";
	body_path 		= "";
	path 			= "";
	query 			= "";
	transfer_encoding = "";
}

Request::~Request(){}
//******************* SETTERS *****//

void    Request::setType(int type)					    { this->type = type; }
void    Request::setMethod(const std::string& method)  {  this->method = method; }
void    Request::setBodyPath(const std::string& body_path) { this->body_path = body_path; }
void    Request::setPath(const std::string& path)          { this->path = UrlDecode(path); }
void    Request::setQuery(const std::string& query) { this->query = query; }
void    Request::setContentLength(const int content_length) { this->content_length = content_length; }
void    Request::setTransferEncoding(const std::string& transfer_encoding) { this->transfer_encoding = transfer_encoding; }

//*****************  GETTERS ********************//

std::string 	Request::getPath()              const { return path; }
std::string 	Request::getQuery()             const { return query; }
int         	Request::getContentLength()     const { return content_length; }
std::string 	Request::getTransferEncoding()  const { return transfer_encoding; }
int             Request::getType() 				const { return type; }
std::string     Request::getMethod()            const { return method; }
std::string     Request::getBodyPath()          const { return body_path; }
bool	        Request::insertHeader(std::string key, std::string value)
{
    if (headers.find(key) != headers.end())  return false;
 	headers.insert(std::make_pair(key, value));
    return true;
}

std::string Request::getHeader(std::string key) {  return   headers.find(key) == headers.end() ?   "" : headers[key]; }

