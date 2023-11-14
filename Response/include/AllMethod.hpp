#pragma	once

#include "GenerateResponse.hpp"
#include "Response.hpp"
#include <string>

#define FILE 0
#define DIRECTORY 1
#define NOT_FOUND 2

class ResponseHandler
{

		public:
			ResponseHandler(Server &ser, Request &req, Response &res);
			~ResponseHandler();

		  std::vector<std::string> indexes;
	  	bool autoindex;
	  	bool isLoacation;
			std::string root;
			Location *location;
			TYPES_MAP	mimeTypes;
			std::string defaultType;

	private:
		Server &ser;
		Request &req;
		Response &res;


	private:
		void ResponseHandlere(Server &ser, Request &req, Response &res);
		bool isLocation(LOCATION_ITT &it);
		void simpleGet();
		std::string findMimeType(std::string path, Server &ser);
		bool checkRedirection();
		std::string GetFileExtension();
		// int	isFile(std::string path, size_t &size);
		void serveFile(std::string path, size_t size);
		void serveDirectory();
		void _extract_resources_from_request();
		bool checkCGI(std::string path);
		void handelCGI();
		std::string GetFileName();


};


