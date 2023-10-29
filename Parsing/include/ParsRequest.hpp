// 
#pragma once
#include "../../HttpElement/include/Request.hpp"
#include "../../parsing/include/GenerateLocation.hpp" // use  for Isinger() function



#define ALLOWED_URL_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%"

class RequestBuffer{

	private:

		std::string									FirstLine;
		std::string									Headers;
		int											level;
		std::string									BodyPath;
		std::ifstream								BodyFile;
		int											Max_BodySize;
		bool										Post;
		int 										Body_offset;
		int											content_length;
		int											Body_insertion_type;
		int											Chunked_size;

	public:

		RequestBuffer(int Max_BodySize);
		RequestBuffer(const RequestBuffer &x);
		RequestBuffer &operator=(const RequestBuffer &x);
		~RequestBuffer();

	public: // Getters.

		std::string									const &getBody() const;
		std::string									const &getFirstLine() const;
		std::string									const &getHeaders() const;
		int											const &getLevel() const;
		int											const &getMax_BodySize() const;
		
};

class ParsRequest
{
public :
	static Request *Pars(RequestBuffer &reqBuff)
	{
		int type = reqBuff.getLevel() > 3 ? Request::BAD_REQUEST : Request::GET;
		Request *req = new(std::nothrow) Request(type);
		if (req == NULL) return NULL;
		if (req->getType() == Request::BAD_REQUEST) return req;


	}

static void ParsFirstLine(Request& req, std::string& line) 
{
    int sp_pos = line.find(' ');
    req.setMethod(line.substr(0, sp_pos));
    int location_pos = line.find(' ', sp_pos + 1);
    std::string location = line.substr(sp_pos + 1, location_pos - sp_pos - 1);
    size_t query_pos = location.find('?');
    if (query_pos != std::string::npos) {
        req.setPath(location.substr(0, query_pos));
        req.setQuery(location.substr(query_pos + 1));
    } else 
        req.setPath(location);
}

static bool isValidKey(std::string key)
{
	std::string special_chars = "_.-";
	for(size_t i = 0; i < key.length(); i++)
	{
		if (std::isalnum(key.at(i)) == false  &&  special_chars.find(key.at(i)) == std::string::npos)
			return false;
	}
	return true;
}

static bool ParsHeaders(Request& req, std::string& line) 
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
		req.insertHeader(key, value);
}

};


