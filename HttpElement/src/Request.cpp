#include "../include/Request.hpp"


Request::Request(int type)
{
	this->type 			= type;
	this->error_code 		= 200;
	this->content_length 	= -1;
	this->method			= "";
	this->path 				= "";
	this->query 			= "";
	this->transfer_encoding = "";
	this->connection        = false;
}

Request::~Request(){}
//******************* SETTERS *****//

void    Request::setType(int type)					    { this->type = type; }
void    Request::setMethod(const std::string &method)  {  this->method = method; }
void    Request::setPath(const std::string& path)          { this->path = UrlDecode(path); }
void    Request::setQuery(const std::string& query) { this->query = query; }
void    Request::setContentLength(const int content_length) { this->content_length = content_length; }
void    Request::setTransferEncoding(const std::string& transfer_encoding) { this->transfer_encoding = transfer_encoding; }
void    Request::setErrorCode(int code) { this->error_code = code; }
void 	Request::setConnection(bool conn) {this->connection = conn;}
void 	Request::setBody(const std::string &path) {this->body = path;}


//*****************  GETTERS ********************//

std::string 	Request::getPath()              const { return path; }
std::string 	Request::getQuery()             const { return query; }
int         	Request::getContentLength()     const { return content_length; }
std::string 	Request::getTransferEncoding()  const { return transfer_encoding; }
int             Request::getType() 				const { return this->type; }
std::string     Request::getMethod()            const { return method; }
int   			Request::getErrorCode() 		const { return this->error_code; }
bool 			Request::getConnection()		const {	return this->connection;}
const std::string 	&Request::getBody() 		const { return this->body; }

const std::map<std::string, std::string> &Request::getHeaders() const { return this->headers;}

//std::map<std::string, std::string> Request::getHeaders() const { return headers; }
bool	        Request::insertHeader(std::string key, std::string value)
{
    if (headers.find(key) != headers.end())  return false;
 	headers.insert(std::make_pair(key, value));
    return true;
}

std::string Request::getHeader(std::string key) {  return   headers.find(key) == headers.end() ?   "" : headers[key]; }

