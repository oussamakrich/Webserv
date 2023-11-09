//############### ALPHABEN ##################
#pragma once
#include "../../include/includes.hpp"
#include "../../Request/Buffer/RequestBuffer.hpp"

class Request
{

private:
	int				type;
	int				error_code;
	int				content_length;
	std::string		method;
	std::string		body;
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
	int                 getErrorCode()			const;
	std::string         getMethod()				const;
	std::string         getPath()				const;
	std::string         getQuery() 				const;
	int                 getContentLength()		const;
	std::string 		getTransferEncoding()	const;
	const std::map<std::string, std::string>	&getHeaders() const;
	bool 				getConnection()			const;  //add connection
	const std::string				&getBody() 	const;


/*********************< setter > *******************/

void	setType(int type);
void	setMethod(const std::string& method);
void	setPath(const std::string& path);
void	setErrorCode(int error_code);
void	setQuery(const std::string& query);
void	setContentLength(const int  content_length);
void	setTransferEncoding(const std::string& transfer_encoding);
void 	setConnection(bool conn);
void 	setBody(const std::string &path);


bool	insertHeader(std::string key, std::string value);
std::string getHeader(std::string key);


};
