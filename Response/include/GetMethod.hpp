

#pragma	once

#include "GenerateResponse.hpp"
#include "Response.hpp"

#define FILE 0
#define DIRECTORY 1
#define NOT_FOUND 2

class GetMethod
{

	public:
		GetMethod(Server &ser, Request &req, Response &res);
		~GetMethod();
	private:
		Server &ser;
		Request &req;
		Response &res;


	private:
		void GetMethode(Server &ser, Request &req, Response &res);
		bool isLocation(LOCATION_ITT &it);
		void simpleGet();
		void locationGet(Location *loc);

		int	isFile(std::string path, size_t &size);
		void serveFile(std::string path, size_t size);

		void serveDirectory();


		void serveDirectoryLoc(Location &loc);
};


