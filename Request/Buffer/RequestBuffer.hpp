#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sys/_types/_size_t.h>
#include <unistd.h>

#include "Byte.hpp"
#include <vector>

#include "../../include/includes.hpp"


#define ALLOWED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%"

#define IS_METHOD_SUPORTED(method)((method == "GET" || method == "DELETE" || method == "POST" ))

class RequestBuffer
{

	private:

		std::string									_method;
		std::string									_uri;
		std::string									_protocol;
		std::string									_headers;
		std::string									_body_path;
		int											_status;
		int											_increment;
	private:

		Byte										_buffer;
		long long									_maxBodySize;
		int											_level;
		long long									_contentLength;
		bool										_found;
		std::string									_boundary;
		int											_chunkSize;
		int											_body_level;
		std::vector<std::string>					_multiform_data_paths;
		std::string									_multiform_data_tmp_path;
		int 										_seek_pos;

	public:

		RequestBuffer(long long MaxBodySize);


	public: //Getters:
		std::string const&							getBody() const;
		int											getBodySize();
		int											getLevel() const;
		std::string	const &							getMethod() const;
		std::string	const &							getURI() const;
		std::string	const &							getProtocol() const;
		std::string	const &							getHeaders() const;

	public: //Setters:

		void										setProtocol(std::string const &Protocol);
		void										setHeaders(std::string const &Headers);
		void										setMethod(std::string const &Method);
		void										setMaxBodySize(int MaxBodySize);
		void										setURI(std::string const &URI);
		void										setBody(char *body, int size);

	public: // Tools:

		int				_first_line_handler();
		int				_headers_handler();
		int				_get_body_level();
		int				_multipart_handler();
		int				_content_length_handler();
		int				_body_handler();
		int				_chunked_handler();
		int				insertBuffer(const char *buffer, int size);
		void			clear();

};

