#pragma once
#include "../../include/includes.hpp"
#include "../../Response/include/Response.hpp"
#include "../../HttpElement/include/Request.hpp"
#include "../../HttpElement/include/Server.hpp"

class Upload
{
	private:
		size_t			output_offset;
		size_t			uploadSize;
		size_t			input_offset;
		size_t			status;
		std::string		outPath;
	public:
		Upload(Server &ser, Request &req, Response &res, Location &loc);
		void multipart(Server &ser, Request &req, Response &res, Location &loc);
		std::string GetFileName(Request &req);
		void Uploader(Server &ser, Request &req, Response &res, Location &loc);
		~Upload();
};


//root + location + upload_path
