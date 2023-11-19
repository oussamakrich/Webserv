#pragma once
#include "../../include/includes.hpp"
#include "../../Response/include/Response.hpp"
#include "../../HttpElement/include/Request.hpp"
#include "../../HttpElement/include/Server.hpp"
#include "../../Request/Buffer/Byte.hpp"
#include <fstream>
#include <string>
#include <sys/_types/_size_t.h>



#define BOUNDARY_READ_SIZE 5000000
class Upload
{
	private:
		Server			&ser;
		Response		&res;
		std::string		outPath;

		long long			_seek_file;
		long long			_pos_of_boundary;
		long long			_pos_of_end_boundary;
		long long 			_end_of_header;
		std::string			_headers;
	private:
		bool _check_file_open_in(std::ifstream &file);
		bool _check_file_open_out(std::ofstream &file);
		bool _extract_headers();
	public:
		void			_get_line(std::ifstream &file);
		void			parseBoundaryFirstTime();
		std::string 	getHeaders(std::ifstream &file);
		Upload(Server &ser, Response &res);

		void Uploader();
		void multipart();
		~Upload();


};
