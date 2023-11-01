//############### ALPHABEN ##################
#pragma once
#include "../../include/includes.hpp"


class Request
{

private:
	int				type;
	int				error_code;
	int				content_length;
	std::string		method;
	std::string		body_path;
	std::string		path;
	std::string		query;
	std::string		transfer_encoding;
	bool 			connection;
	std::map<std::string, std::string> headers;

public :

	enum REQ_TYPE {NOT_SUPPORTED = -2 ,INVALID_REQUEST = -1 , GET, POST, DELETE};
	Request	(int type);
	~Request();

/*********************< setter > *******************/

	int                 getType()				const;
	int                 getBodySize()			const;
	int                 getErrorCode()			const;
	std::string         getMethod()				const;
	std::string         getBodyPath() 			const;
	std::string         getPath()				const;
	std::string         getQuery() 				const;
	int                 getContentLength()		const;
	std::string getTransferEncoding()			const;
	std::map<std::string, std::string> getHeaders() const;
	bool 				getConnection()			const;  //add connection 


/*********************< setter > *******************/

void	setType(int type);
void	setBodySize(int body_size);
void	setMethod(const std::string& method);
void	setBodyPath(const std::string& body_path);
void	setPath(const std::string& path);
void	setErrorCode(int error_code);
void	setQuery(const std::string& query);
void	setContentLength(const int  content_length);
void	setTransferEncoding(const std::string& transfer_encoding);
void 	setConnection(bool conn);

bool	insertHeader(std::string key, std::string value);
std::string getHeader(std::string key);


};
