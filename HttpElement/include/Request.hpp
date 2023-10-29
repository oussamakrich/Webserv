#pragma once
#include "../../include/includes.hpp"
#include <map>

class Request
{

private:
    int type;
	int error_code;
	int body_size;
	int content_length;
	std::string method;
	std::string body_path;
    std::string path;
	std::string query;

	std::string transfer_encoding;
	std::map<std::string, std::string> headers;

public :

	enum REQ_TYPE {BAD_REQUEST = -1 , GET, POST, PUT, DELETE, HEAD, OPTIONS, TRACE, CONNECT};
	Request	(int type);
    int getType() const;
    int getBodySize() const;
   int getErrorCode() const;
    std::string getMethod() const;
    std::string getBodyPath() const;
    std::string getPath() const;
    std::string getQuery() const;
    int getContentLength() const;
    std::string getTransferEncoding() const;
    std::map<std::string, std::string> getHeaders() const;

    // Setter prototype
  
    // Setter prototypes
    void setType(int type);
    void setBodySize(int body_size);
    void setMethod(const std::string& method);
    void setBodyPath(const std::string& body_path);
    void setPath(const std::string& path);
  	void setErrorCode(int error_code);
    void setQuery(const std::string& query);
    void setContentLength(const int  content_length);
    void setTransferEncoding(const std::string& transfer_encoding);
    void setHeaders(const std::map<std::string, std::string>& headers);
	bool insertHeader(std::string key, std::string value);
	std::string getHeader(std::string key);

	
};