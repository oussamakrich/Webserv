#pragma once

#include "../../include/includes.hpp"


class Response{

	private:
		int code;
		std::string version;
		std::string msg;
		std::vector<std::string> headers;
		char *buffer;
		int bufferSize;
		std::string HeaderAndStart;
	
	public:
		bool stillSend;
		std::string path;
		int pos;
		int lastSend;
		

	public:
		Response();

	public:
		int getCode();
		std::string getVersion();
		std::string getMsg();
		std::vector<std::string> getHeaders();
		char *getBuffer();
		int getBufferSize();

		void setCode(int code);
		void setVersion(std::string version);
		void setMsg(std::string msg);
		void setHeaders(std::vector<std::string> headers);
		void setHeadr(std::string header);
		void setBuffer(char *buffer, int size);
		void setHeaderAndStart(std::string header);


};
