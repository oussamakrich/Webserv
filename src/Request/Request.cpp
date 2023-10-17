#include "../../include/Request.hpp"
#include <sstream>
#include <string>
#include <variant>

// Canonical Form:

Request::~Request() {}

Request::Request() {}

Request::Request(const Request &copy) {
	*this = copy;
}

Request &Request::operator=(const Request &copy) {
	if (this != &copy) {
		this->method = copy.method;
		this->path = copy.path;
		this->version = copy.version;
		this->headers = copy.headers;
		this->body = copy.body;
	}
	return *this;
}

// End Canonical Form.

// Getters:

std::string const &Request::getMethod() const {
	return this->method;
}

std::string const &Request::getPath() const {
	return this->path;
}

std::string const &Request::getVersion() const {
	return this->version;
}

std::string const &Request::getHeader(std::string const &key) const {

	if (this->headers.find(key) == this->headers.end())
		return this->headers.find("Host")->second;
	return this->headers.find(key)->second;
}

std::string const &Request::getBody() const {
	return this->body;
}

// Setters:

void Request::setMethod(std::string const &method) {
	this->method = method;
}

void Request::setPath(std::string const &path) {
	this->path = path;
}

void Request::setVersion(std::string const &version) {
	this->version = version;
}

void Request::setHeaders(std::pair<std::string, std::string> &headers) {
	this->headers.insert(headers);
}

void Request::setBody(std::string const &body) {
	this->body = body;
}

// End Getters and Setters.


// Constructor:

Request::Request(std::string const &request)
{
	if (request.empty())
		throw std::runtime_error("Request is empty.");
	std::stringstream ss(request);
	ss>>this->method;
	ss>>this->path;
	if (path.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
		throw std::runtime_error("Invalid path.");
	ss>>this->version;
	ss.clear();
	ss.str(request);
	std::string line;

	while (std::getline(ss, line))
	{
		if (line.find(method) == 0)
			continue;
		if (line.empty())
			break;
		std::string key;
		std::string value;
		std::stringstream ss2(line);
		std::getline(ss2, key, ':');
		std::getline(ss2, value);
		this->headers.insert(std::pair<std::string, std::string>(key, trim(value)));
	}
	if (this->headers.find("Content-Length") != this->headers.end())
	{
		ss.clear();
		ss.str(this->headers.find("Content-Length")->second);
		ss>>this->content_length;
	}
	std::cout << method << " " << path << " " << version << std::endl;
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
}
