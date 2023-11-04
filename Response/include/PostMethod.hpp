#pragma	once

#include "GenerateResponse.hpp"
#include "Response.hpp"


class PostMethod {

	private:
		Server &ser;
		Request &req;
		Response &res;


	private:
		bool isLocation(LOCATION_ITT &it);
		// void simplePost();
		// void locationPost(Location *loc);
		// int isFile(std::string path, size_t &size);
		// void serveFile(std::string path, size_t size);

	public:
		PostMethod(Server &ser, Request &req, Response &res);
		~PostMethod();

};



