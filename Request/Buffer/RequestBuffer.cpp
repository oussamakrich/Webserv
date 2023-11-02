#include "RequestBuffer.hpp"
#include <csetjmp>
#include <sstream>
#include <string>


RequestBuffer::~RequestBuffer() {}


RequestBuffer::RequestBuffer(int MaxBodySize)
{
	this->MaxBodySize = MaxBodySize;
	this->t_buffer.buffer = NULL;
	this->t_buffer.size = 0;
	this->Level = 0;
	this->Success = 0;
	this->bodyType = -1;
};

/************************************ Tools: ***********************************/

char *RequestBuffer::strjoin(char *s1, char *s2, int size1, int size2)
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
	if (size1 > 0)
		delete[] s1;
	return (tmp);
}


char *RequestBuffer::strDuplicate(char *str, int size)
{
	char	*tmp;
	int		i;

	if (size == 0)
		return (NULL);
	i = 0;
	tmp = new char[size + 1];
	while (i < size)
	{
		tmp[i] = str[i];
		i++;
	}
	return (tmp);
}


char *RequestBuffer::substr(char *str, int start, int end)
{
	char	*tmp;
	int		i;

	if (end < start)
		return (NULL);
	i = 0;

	tmp = new char[end - start];
	while (start < end)
	{
		tmp[i] = str[start];
		i++;
		start++;
	}

	if (end - start > 0)
		delete[] str;
	return (tmp);
}

int RequestBuffer::find(char *str, const char *to_find, int strSize, int to_findSize)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (i < strSize)
	{
		if (str[i] == to_find[j])
		{
			while (str[i + j] == to_find[j] && j < to_findSize)
				j++;
			if (j == to_findSize)
				return (i);
			j = 0;
		}
		i++;
	}
	return (-1);
}

bool	RequestBuffer::strLenCmp(char *str1, const char *str2, int len)
{
	int i = 0;

	while (i < len)
	{
		if (str1[i] != str2[i])
			return false;
		i++;
	}
	return true;
}

/*******************************************************************************/






/*********************************** Methods: **********************************/

int	RequestBuffer::insertBuffer(char *buffer, int size)
{
	switch (Level) {
		case 0:
			RequestBuffer::InsertFirstLine(buffer, size);
			break;
		case 1:
			RequestBuffer::InsertHeaders(buffer, size);
		// 	break;
		// case 2:
		// 	RequestBuffer::InsertBody(buffer, size);
		// 	break;
		// default:
		// 	break;
	}
	return (RequestBuffer::Success);
}


/*
	1 -> join the buffer with the firstLine string.
	2 -> find the end of the firstLine string.
	3 -> if the end of the firstLine string is found, then we start pars the firstLine.
*/
int RequestBuffer::InsertFirstLine(char *buffer, int size)
{
	int		pos;
	int		count = 0;
	std::string tmp;
	RequestBuffer::firstLine += std::string(buffer, size);

	if ((pos = RequestBuffer::firstLine.find("\r\n")) != std::string::npos)
	{
		firstLine = firstLine.substr(0, pos);
		for (int i = 0; i < pos; i++)
			if (RequestBuffer::firstLine[i] == ' ')		count++;
		if (count != 2) RequestBuffer::Success = 405;
		std::stringstream ss(firstLine);
		while (ss>>tmp)
		{
			if (Method.empty())			Method = tmp;
			else if (URI.empty())		URI = tmp;
			else if (Protocol.empty())	Protocol = tmp;
		}
		if (Method.empty() || URI.empty() || Protocol.empty()) RequestBuffer::Success = 405;
		if (Protocol.find_first_not_of(ALLOWED_CHARS) != std::string::npos) RequestBuffer::Success = 405;
		if (IS_METHOD_SUPORTED(Method) == false) RequestBuffer::Success = 405;
		if (Protocol != "HTTP/1.1") RequestBuffer::Success = 405;

		size -= pos + 2;
		if (size <= 0)
		{
			return (Level++);
		}
		if (size >= 2 && RequestBuffer::strLenCmp((buffer + pos + 2), "\r\n", 2) == true)
			return (RequestBuffer::Success = 1, 1);
		RequestBuffer::Level++;
		RequestBuffer::InsertHeaders((buffer + pos + 2), size);
	}
	return (RequestBuffer::Level);
}


int	RequestBuffer::InsertHeaders(char *buffer, int size)
{
	int	LineBreak;

	if (buffer[0] == '\r' || buffer[0] == '\n')
	{
		int i = 0;
		while (i < size && (buffer[i] == '\r' || buffer[i] == '\n'))	i++;
		std::string test = Headers.substr(Headers.length() - 3, 3);

		test += std::string(buffer, i);
		if ((LineBreak = test.find("\r\n\r\n")) != std::string::npos)
		{
			if (LineBreak > 2) {}
			else
			{
				char *tmp = NULL;
				switch (LineBreak)
				{
					case 0:	tmp = RequestBuffer::strDuplicate(buffer, 1);	break;
					case 1:	tmp = RequestBuffer::strDuplicate(buffer, 2);	break;
					case 2:	tmp = RequestBuffer::strDuplicate(buffer, 3);	break;
					default:	break;
				}
				if (tmp != NULL)
				{
					RequestBuffer::Headers += std::string(tmp, LineBreak+1);
					delete [] tmp;
				}
				Headers.erase(std::remove(Headers.begin(), Headers.end(), '\r'), Headers.end());
				RequestBuffer::Level++;
				if (this->Method != "POST")
					return (this->Success = 1, 1);
				else
				{
					size -= LineBreak+1;
					if (size <= 0)
						return (1);
					buffer += LineBreak+1;
					RequestBuffer::InsertBody(buffer, size);
					return (1);
				}
			}
		}
	}
	LineBreak = RequestBuffer::find(buffer, "\r\n\r\n", size, 4);

	if (LineBreak != -1)
	{
		RequestBuffer::Headers += std::string(buffer, LineBreak);
		size -= LineBreak + 4;
		if (size <= 0)
		{
			Headers.erase(std::remove(Headers.begin(), Headers.end(), '\r'), Headers.end());
			RequestBuffer::Level++;
			if (this->Method != "POST")
				return (RequestBuffer::Success = 1, 1);
			return (1);
		}
		Headers.erase(std::remove(Headers.begin(), Headers.end(), '\r'), Headers.end());
		RequestBuffer::Level++;
		RequestBuffer::InsertBody((buffer + LineBreak + 4), size);
	}
	else
	{
		RequestBuffer::Headers += std::string(buffer, size);
	}

	return (1);
}

/*
	1: content-Length.
	2: Transfer-Encoding.
	3: boundary.
	-1: Error
*/
int	RequestBuffer::GenerateBodyType()
{
	int pos;
	if ((pos = this->Headers.find("Content-Length: ")) != std::string::npos)
	{
		this->contentLength = std::atoi(
			Headers.substr((pos + 16), Headers.find('\r', pos + 16) - (pos + 16)).c_str()
		);
		return (1);
	}
	else if ((pos = this->Headers.find("Transfer-Encoding: chunked")) != std::string::npos)
		return (2);
	else if ((pos = this->Headers.find("Content-Type: multipart/form-data; boundary=")) !=std::string::npos)
	{
		this->boundary = Headers.substr((pos + 44), Headers.find("\"") - (pos + 44));
		return (3);
	}
	return (-1);
}

int	RequestBuffer::InsertContentLengthBody(char *buffer, int size)
{
	if (RequestBuffer::t_buffer.buffer == NULL)
	{
		RequestBuffer::t_buffer.buffer = RequestBuffer::strDuplicate(buffer, size);
		RequestBuffer::t_buffer.size = size;
		this->contentLength -= size;
	}
	else
	{
		RequestBuffer::t_buffer.buffer = RequestBuffer::strjoin(buffer, RequestBuffer::t_buffer.buffer, size, RequestBuffer::t_buffer.size);
		RequestBuffer::t_buffer.size += size;
		this->contentLength -= size;
	}
	if (this->contentLength <= 0)
	{
		RequestBuffer::Success = 1;
	}
	return (1);
}

int	RequestBuffer::InsertBody(char *buffer, int size)
{
	if (RequestBuffer::bodyType == -1) bodyType = RequestBuffer::GenerateBodyType();

	switch (RequestBuffer::bodyType)
	{
		case 1:
			if (contentLength > MaxBodySize)
			{
				RequestBuffer::Success = 413; // Request Entity Too Large
			}
			else
				RequestBuffer::InsertContentLengthBody(buffer, size);
			break;
		// case 2:

		// 	break;
		// case 3:

		// 	break;
		default:
			break;
	}
	return 1;
}

void RequestBuffer::clear()
{
	if (this->t_buffer.buffer != NULL)
		delete[] this->t_buffer.buffer;
	this->t_buffer.buffer = NULL;
	this->t_buffer.size = 0;
	this->Level = 0;
	this->Success = 0;
	this->bodyType = -1;
	this->firstLine.clear();
	this->Headers.clear();
	this->Method.clear();
	this->URI.clear();
	this->Protocol.clear();
	this->contentLength = 0;
	this->boundary.clear();
}











/*********************************** Getters: ***********************************/

char *RequestBuffer::getBody() const {return (this->t_buffer.buffer);}

int RequestBuffer::getBodySize() const {return (this->t_buffer.size);}

int RequestBuffer::getMaxBodySize() const {return (this->MaxBodySize);}

std::string const &RequestBuffer::getMethod() const {return (this->Method);}

std::string const &RequestBuffer::getURI() const {return (this->URI);}

std::string const &RequestBuffer::getProtocol() const {return (this->Protocol);}

std::string const &RequestBuffer::getHeaders() const {return (this->Headers);}

/*******************************************************************************/





/*********************************** Setters: ***********************************/

void RequestBuffer::setBody(char *body, int size)
{
	if (this->t_buffer.buffer != NULL)
		delete[] this->t_buffer.buffer;
	this->t_buffer.buffer = body;
	this->t_buffer.size = size;
}

void RequestBuffer::setMaxBodySize(int MaxBodySize) {this->MaxBodySize = MaxBodySize;}

void RequestBuffer::setMethod(std::string const &Method) {this->Method = Method;}

void RequestBuffer::setURI(std::string const &URI) {this->URI = URI;}

void RequestBuffer::setProtocol(std::string const &Protocol) {this->Protocol = Protocol;}

void RequestBuffer::setHeaders(std::string const &Headers) {this->Headers = Headers;}

/*******************************************************************************/
