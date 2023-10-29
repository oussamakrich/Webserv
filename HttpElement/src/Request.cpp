#include "../include/Request.hpp"

//** SETTERS **//
void Request::setType(int type)					{ this->type = type; }
void Request::setBodySize(int body_size) 		{ this->body_size = body_size; }	
void Request::setMethod(const std::string& method) {  this->method = method; }
void Request::setBodyPath(const std::string& body_path) { this->body_path = body_path; }
void Request::setPath(const std::string& path) { this->path = path; }
void Request::setQuery(const std::string& query) { this->query = query; }
void Request::setContentLength(const int content_length) { this->content_length = content_length; }
void Request::setTransferEncoding(const std::string& transfer_encoding) { this->transfer_encoding = transfer_encoding; }
//** GETTERS **//
int Request::getType() 				const { return type; }
int Request::getBodySize() const { return body_size; }
std::string Request::getMethod() const { return method; }
std::string Request::getBodyPath() const { return body_path; }
std::string Request::getPath() const { return path; }
std::string Request::getQuery() const { return query; }
int Request::getContentLength() const { return content_length; }
std::string Request::getTransferEncoding() const { return transfer_encoding; }
//** MAP GETTERS **//
bool Request::insertHeader(std::string key, std::string value) 
{
    if (headers.find(key) != headers.end())  return false;   
 	headers.insert(std::make_pair(key, value));
    return true;
}
//** MAP INSERTY **//
std::string Request::getHeader(std::string key) {  return   headers.find(key) == headers.end() ?   "" : headers[key]; }

