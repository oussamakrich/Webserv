

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
		bool isLocation();
		void simpleGet();
		void locationGet();

		int	isFile(std::string path, size_t &size);
		void serveFile(std::string path, size_t size);
		// static void sendReminder(Response &req);

		


};


