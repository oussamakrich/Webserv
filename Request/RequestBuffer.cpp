#include "RequestBuffer.hpp"
#include <algorithm>
#include <cstring>
#include <sstream>
#include <string.h>
#include <string>


char *ft_strdup(const char *s1)
{
	char *s2;
	int i;

	i = 0;
	if (!s1)
		return (NULL);
	while (s1[i])
		i++;
	s2 = new char[i + 1];
	i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = s1[i];
	return (s2);
}



/****************************************************************************************************/
RequestBuffer::RequestBuffer(int Max_BodySize){
	this->Max_BodySize = Max_BodySize;
	this->level = 0;
	this->Body_offset = 0;
	this->content_length = -1;
	this->Chunked_size = -1;

}

RequestBuffer::RequestBuffer(const RequestBuffer &x){
	this->Max_BodySize = x.Max_BodySize;
	this->level = x.level;
	this->Body_offset = 0;
	this->content_length = -1;
}

RequestBuffer &RequestBuffer::operator=(const RequestBuffer &x){

	if (this == &x)
		return (*this);
	this->Max_BodySize = x.Max_BodySize;
	this->level = x.level;
	this->Body_offset = x.Body_offset;
	this->content_length = x.content_length;
	return *this;
}

RequestBuffer::~RequestBuffer() {}
/****************************************************************************************************/



/*                                                Getters                                           */
/****************************************************************************************************/
int const &RequestBuffer::getMax_BodySize() const				{return this->Max_BodySize;}
std::string const &RequestBuffer::getFirstLine() const			{return this->FirstLine;}
std::string const &RequestBuffer::getHeaders() const			{return this->Headers;}
int const &RequestBuffer::getLevel() const						{return this->level;}
std::string const &RequestBuffer::getBody() const				{return this->BodyPath;}
/****************************************************************************************************/


/*                                                Setters                                           */
/****************************************************************************************************/
void RequestBuffer::setMax_BodySize(int const &Max_BodySize)	{this->Max_BodySize = Max_BodySize;}
void RequestBuffer::setFirstLine(std::string const &FirstLine)	{this->FirstLine = FirstLine;}
void RequestBuffer::setHeaders(std::string const &Headers)		{this->Headers = Headers;}
void RequestBuffer::setLevel(int const &level)					{this->level = level;}
void RequestBuffer::setBody(char* const &Body)					{}
/****************************************************************************************************/


/*                                                Methods                                           */
/****************************************************************************************************/

int RequestBuffer::insertToBuffer(char *buffer, int size)
{
	switch (this->level)
	{
		case 0:
			this->insertFirstLine(buffer, size);
			break;
		case 1:
			this->insertHeaders(buffer, size);
			break;
		case 2:
			this->insertBody(buffer, size);
			break;
		default:
			break;
	}
	return level;
}

/****************************************************************************************************/





/*                                                Tools                                             */
/****************************************************************************************************/

void RequestBuffer::parseFirstLine()
{
	FirstLine = FirstLine.substr(0, FirstLine.find("\r"));
	Post = false;
	std::string line;
	std::stringstream ss(this->FirstLine);
	int i = 0;
	while (ss>>line)
	{
		if (line == "POST")
			Post = true;
		if (i == 0 && !IS_METHOD_SUPORTED(line))	{this->level = 405; return;}
		else if (i == 1 && line[0] != '/' && (line.find_first_not_of(ALLOWED_URL_CHARS) != std::string::npos))	{this->level = 400; return;}
		else if (i == 2 && line != "HTTP/1.1")		{this->level = 505; return;}
		else if (i > 2)								{this->level = 400; return;}
		i++;
	}
}

void RequestBuffer::insertFirstLine(char *buffer, int size)
{
	int i = 0;
	while (i < size && buffer[i] != '\n') i++;

	this->FirstLine += (std::string(buffer, i));
	if (i < size && buffer[i] == '\n')
	{
		parseFirstLine();
		this->level++;
		buffer += i + 1;
		size -= i + 1;
		this->insertHeaders(buffer, size);
	}
}

void RequestBuffer::insertHeaders(char *buffer, int size)
{
	char *pos = strstr(buffer, "\r\n\r\n");
	if (pos)
	{
		this->Headers += std::string(buffer, pos - buffer);
		this->level++;
		if (Post == true)
		{
			if (this->Headers.find("Content-Length: ") != std::string::npos)
			{
				this->content_length = atoi(this->Headers.substr(this->Headers.find("Content-Length: ") + 16, this->Headers.find("\r\n", this->Headers.find("Content-Length: ") + 16) - this->Headers.find("Content-Length: ") - 16).c_str());
				if (this->content_length > this->Max_BodySize)
				{
					this->level = 413;
					return;
				}
				this->Body_insertion_type = 1;

				this->insertBodyContentLength(pos + 4, size - (pos - buffer) - 4);
			}
			else if (this->Headers.find("Transfer-Encoding: chunked") != std::string::npos)
			{
				this->Body_insertion_type = 2;
				this->insertBodyChunked(pos + 4, size - (pos - buffer) - 4);
			}
			else
			{
				this->level = 411;
				return;
			}
		}
	}
	else
		this->Headers += std::string(buffer, size);
}

std::string RequestBuffer::GenerateBodyPath()
{
    std::string path = "/tmp/0.tmp";
    int i = 1;
    while (access(path.c_str(), F_OK) == 0)
    {
        std::stringstream ss;
        ss << i;
        path = "/tmp/" + ss.str() + ".tmp";
        i++;
    }
    return path;
}

void RequestBuffer::insertBodyContentLength(char *buffer, int size)
{
	if (this->Body_offset + size > this->content_length && this->content_length != -1 && Body_offset != 0)
	{
		this->level = 413;
		return;
	}
	if (this->BodyPath.empty()) this->BodyPath = this->GenerateBodyPath();
	std::ofstream file(this->BodyPath.c_str(), std::ios::app);
	if (content_length < size)
	{
		file.write(buffer, content_length);
		file.close();
		this->level++;
		return;
	}
	else {
		file.write(buffer, size);
		content_length -= size;
		file.close();
	}
}


void RequestBuffer::insertBody(char *buffer, int size)
{
	switch (this->Body_insertion_type)
	{
		case 1:
			this->insertBodyContentLength(buffer, size);
			break;
		case 2:
			this->insertBodyChunked(buffer, size);
			break;
		// case 3:
		// 	this->insertBodyBoundary(buffer, size);
		// 	break;
		default:
			break;
	}
}


int ft_strstr(char *buffer, char const *toFind, int bufferSize, int toFindSize)
{
    int i = 0;
    while (i < bufferSize)
    {
        if (buffer[i] == toFind[0])
        {
            int j = 0;
            while (j < toFindSize && i + j < bufferSize && buffer[i + j] == toFind[j])
                j++;
            if (j == toFindSize)
                return i;
        }
        i++;
    }
    return -1;
}

int RequestBuffer::getChunkedSize(char *buffer, int size)
{
	int					chunked_size;
	int					i;

	i = 0;
	while (i < size && buffer[i] == '\n' || buffer[i] == '\r') i++;

	int n = ft_strstr(buffer + i, "\r\n", size - i, 2);
	if (n < 0)
		return -1;
	std::stringstream	ss(std::string(buffer + i, n));

	if (!(ss >> std::hex >> chunked_size))
	{
		return -1;
	}
	return chunked_size;
}



void RequestBuffer::insertBodyChunked(char *buffer, int size)
{

	if (BodyPath.empty()) BodyPath = GenerateBodyPath();
	if (Chunked_size <= 0)
	{
		Chunked_size = getChunkedSize(buffer, size);
		if (Chunked_size < 0)			{this->level = 400; return;}
		else if (Chunked_size == 0)		{this->level++;return;}
		buffer += ft_strstr(buffer, "\n", size, 1) + 1;
	}
	while (Chunked_size > 0)
	{
		if (Chunked_size > size)
		{
			std::ofstream file(this->BodyPath.c_str(), std::ios::app);
			file.write(buffer, size);
			file.close();
			Chunked_size -= size;
			return;
		}
		else
		{
			std::ofstream file(this->BodyPath.c_str(), std::ios::app);
			file.write(buffer, Chunked_size);
			file.close();
			buffer += Chunked_size;
			size -= Chunked_size;
			if (size == 0)
			{
				Chunked_size = 0;
				return;
			}
			else
			{
				std::cout << "Chunked_size: " << Chunked_size << " buffer[0]: "<< (int)buffer[0] << std::endl;
				Chunked_size = getChunkedSize(buffer, size);
				std::cout << "Chunked_size: " << Chunked_size << " buffer[0]: "<< (int)buffer[0] << std::endl;
				if (Chunked_size < 0)			{this->level = 402; return;}
				else if (Chunked_size == 0)		{this->level++;return;}
				buffer += ft_strstr(buffer, "\n", size, 1) + 1;
			}
		}
	}
}

/***************************************************************/
