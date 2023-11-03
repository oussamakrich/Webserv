// #pragma once
// #include <iostream>
// #include <string>

// #define ALLOWED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%"

// #define IS_METHOD_SUPORTED(method)((method == "GET" || method == "DELETE" || method == "POST" ))

// class RequestBuffer
// {

// 	private:

// 		int											bodyType;// default: -1, 1: content-Length, 2: Transfer-Encoding: chunked, 3:
// 		int											Level;
// 		int											MaxBodySize;// this will store the max body size.
// 		std::string									firstLine;
// 		int											contentLength;
// 		std::string									boundary;


// 	private:

// 		struct buffer{char *buffer;int size;}		t_buffer;// this will store the request body.
// 		std::string									Method;// this will store the method.
// 		std::string									URI; // this will store the URI.
// 		std::string									Protocol; // this will store the protocol.
// 		std::string									Headers; // this will store the headers.
// 		int											Success;


// 	private:
// 		RequestBuffer();
// 		RequestBuffer &operator=(const RequestBuffer &x);
// 		RequestBuffer(const RequestBuffer &x);



// 	public:

// 		RequestBuffer(int MaxBodySize);
// 		~RequestBuffer();


// 	public: //Getters:
// 		char										*getBody() const;
// 		int											getBodySize() {return (this->t_buffer.size);};
// 		int											getBodySize() const;
// 		int											getMaxBodySize() const;
// 		int											getLevel() const{return Success;};
// 		std::string	const &							getMethod() const;
// 		std::string	const &							getURI() const;
// 		std::string	const &							getProtocol() const;
// 		std::string	const &							getHeaders() const;

// 	public: //Setters:
// 		void										setProtocol(std::string const &Protocol);
// 		void										setHeaders(std::string const &Headers);
// 		void										setMethod(std::string const &Method);
// 		void										setMaxBodySize(int MaxBodySize);
// 		void										setURI(std::string const &URI);
// 		void										setBody(char *body, int size);


// 	private: // Tools:

// 		int											find(char *str, const char *to_find, int strSize, int to_findSize);
// 		char										*strjoin(char *s1, char *s2, int size1, int size2);
// 		char										*substr(char *str, int start, int end);
// 		char										*strDuplicate(char *str, int size);
// 		bool										strLenCmp(char *str1,const char *str2, int len);
// 	private: // Inserters:

// 		int											InsertFirstLine(char *buffer, int size);
// 		int											InsertHeaders(char *buffer, int size);
// 		int											InsertBody(char *buffer, int size);
// 		int											GenerateBodyType();
// 		// int											InsertChunkedBody(char *buffer, int size);
// 		int											InsertContentLengthBody(char *buffer, int size);

// 	public:
// 		int											insertBuffer(char *buffer, int size);
// 		void										clear();
// };

#pragma once
#include <iostream>
#include <string>

#define ALLOWED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%"

#define IS_METHOD_SUPORTED(method)((method == "GET" || method == "DELETE" || method == "POST" ))

class RequestBuffer
{

	private:

		int											bodyType;// default: -1, 1: content-Length, 2: Transfer-Encoding: chunked, 3:
		int											Level;
		int											MaxBodySize;// this will store the max body size.
		std::string									firstLine;
		int											contentLength;
		std::string									boundary;
		bool										found;
		char										*tmpString;
		char										*freeHelper;
		struct Tmp{char *tmp;int size;}				t_tmp;// this is a temporary buffer to store the body.


	private:

		struct buffer{char *buffer;int size;}		t_buffer;// this will store the request body.
		std::string									Method;// this will store the method.
		std::string									URI; // this will store the URI.
		std::string									Protocol; // this will store the protocol.
		std::string									Headers; // this will store the headers.
		int											Success;


	private:
		RequestBuffer();
		RequestBuffer &operator=(const RequestBuffer &x);
		RequestBuffer(const RequestBuffer &x);



	public:

		RequestBuffer(int MaxBodySize);
		~RequestBuffer();


	public: //Getters:
		int											getLevel() const;
		char										*getBody() const;
		int											getBodySize() {return (this->t_buffer.size);};
		int											getBodySize() const;
		int											getMaxBodySize() const;
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


	private: // Tools:

		int											find(char *str, const char *to_find, int strSize, int to_findSize);
		char										*strjoin(char *s1, char *s2, int size1, int size2);
		char										*substr(char *str, int start, int end);
		char										*strDuplicate(char *str, int size);
		bool										strLenCmp(char *str1,const char *str2, int len);
		char										*nullTerminate(char *str, int size);

	private: // Inserters:

		int											InsertFirstLine(char *buffer, int size);
		int											InsertHeaders(char *buffer, int size);
		int											InsertBody(char *buffer, int size);
		int											GenerateBodyType();
		int											InsertTransferEncodingBody(char *buffer, int size);
		int											InsertContentLengthBody(char *buffer, int size);
		int											ParseChunkedBodyAndFillBuffer();
		int											GetChunkSize();
		int											FillBufferBodyChunked();
	public:
		int											insertBuffer(char *buffer, int size);
		void										clear();

};


