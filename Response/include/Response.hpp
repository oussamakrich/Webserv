#pragma once

#include "../../include/includes.hpp"
#include "../../HttpElement/include/Cgi.hpp"
#include "../../HttpElement/include/Location.hpp"
#define R_READ 50000
#define VECT_STR std::vector<std::string>
#define VECT_ERRORPIR std::vector<ERRPAGE_PAIR>

class Server;

class Response{

	private:
		int code;
		std::string version;
		std::string msg;
		std::vector<std::string> headers;
		char *buffer;
		int bufferSize;


	public:
		std::string			_file_uploading;
		std::string 		_source_file;
		std::string			_boundary;
		std::string 		_upload_path;
		Location			*location;
		Byte				byte;
		long long			_seek_pos;
		std::string			_single_file_name;
		bool				_is_multipart_form;

	public:

		std::string HeaderAndStart;
		bool iHaveUpload;// false
		bool stillSend;// sttll send the reminder
		std::string path; // path of file to serv it when we send the reminder
		int lastSend; // Not now
		VECT_ERRORPIR errorPage;
		long long pos;
		bool isCGI;
		bool errrCgi;
		t_cgiInfo cgiInfo;
		bool		errorInSend;
		std::string		multiDataFormPath;


	public:

		Response(int fd);
		~Response();
		int fd; // fd of client

	public:
		bool getUploadStat() {return iHaveUpload;};
		int getCode();
		std::string getVersion();
		std::string getMsg();
		std::vector<std::string> getHeaders();
		char *getBuffer();
		int getBufferSize();
		std::string *getHeaderAndStart();

		void setCode(int code);
		void setVersion(std::string version);
		void setMsg(std::string msg);
		void setHeaders(std::vector<std::string> headers);
		void setHeadr(std::string header);
		void setBuffer(char *buffer, int size);
		void setHeaderAndStart(std::string header);
		void setStellUpload(bool iHaveUpload) {this->iHaveUpload = iHaveUpload;};



		bool sendResponse();
		bool ReminderResponse();
		bool CgiResponse(Request &req);
		bool CgiRead(Request &req);
		void CgiHeaders(Request &req, std::ifstream &file, size_t sizeOfFile);

		void sendErrorResponse(Server &ser, int fd);
		void HandelLocation();

		char *strjoin(const char *s1, char *s2, size_t size1, size_t size2)
		{
			char	*tmp;
			int		i;
			int		j;

			i = 0;
			j = 0;
			tmp = new char[size1 + size2 + 1];
			while (i < size1)
			{
				tmp[i] = s1[i];
				i++;
			}
			while (j < size2)
			{
				tmp[i + j] = s2[j];
				j++;
			}
			if (size2 > 0)
				delete[] s2;
			return (tmp);
		}


};
