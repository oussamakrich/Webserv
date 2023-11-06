#include "RequestBuffer.hpp"
#include <sstream>
#include <fstream>

std::string const&							RequestBuffer::getBody() const {return _body_path;};
int											RequestBuffer::getBodySize() {return (_buffer.size());};
int											RequestBuffer::getLevel() const {return _status;};
std::string	const &							RequestBuffer::getMethod() const {return _method;};
std::string	const &							RequestBuffer::getURI() const {return _uri;};
std::string	const &							RequestBuffer::getProtocol() const {return _protocol;};
std::string	const &							RequestBuffer::getHeaders() const {return _headers;};


void										RequestBuffer::setProtocol(std::string const &Protocol) {_protocol = Protocol;};
void										RequestBuffer::setHeaders(std::string const &Headers) {_headers = Headers;};
void										RequestBuffer::setMethod(std::string const &Method) {_method = Method;};
void										RequestBuffer::setMaxBodySize(int MaxBodySize) {_maxBodySize = MaxBodySize;};
void										RequestBuffer::setURI(std::string const &URI) {_uri = URI;};
void										RequestBuffer::setBody(char *body, int size) {_body_path = std::string(body, size);};


void RequestBuffer::clear()
{
	_buffer.clear();
	_level = 0;
	_contentLength = -1;
	_status = 0;
	_chunkSize = -1;
	_boundary = "";
	_body_level = -1;
	_status = 0;
}

RequestBuffer::RequestBuffer(int MaxBodySize)
{
	_maxBodySize = MaxBodySize;
	_level = 0;
	_contentLength = -1;
	_status = 0;
	_chunkSize = -1;
	_boundary = "";
	_body_level = -1;
}

int RequestBuffer::_first_line_handler()
{
	int _pos = _buffer.find("\r\n", 2);
	if (_pos == -1)						return 0;
	char *_tmp = nullTerminate(_buffer.substr(0, _pos), _pos);
	std::string first_line = _tmp;
	if (_tmp != NULL) delete [] _tmp;
	_buffer.resize(_pos + 2);
	_pos = 0;
	// calculate the number of space in the first line.
	for (std::string::iterator it = first_line.begin(); it != first_line.end(); it++)
		if (*it == ' ')
			_pos++;
	// if the number of space is not 2, return 400: bad request.
	if (_pos != 2)						return 400;
	std::stringstream ss(first_line);
	ss >> _method >> _uri >> _protocol; // insert first line.
	if (!IS_METHOD_SUPORTED(_method))	return 501; // not implemented.
	// if the protocol is not HTTP/1.1, return 505: HTTP version not supported.
	if (_protocol != "HTTP/1.1")		return 505;
	// URI too long.
	if (_uri.size() > 2048)				return 414;
	// if the URI contains not allowed characters, return 400: bad request.
	if (_uri.find_first_not_of(ALLOWED_CHARS) != std::string::npos) return 400;
	if ((_pos = _buffer.find("\r\n", 2)) == 0)	return 1; // This mean that the request does not have headers.
	_level++;
	_headers_handler();
	_found = true;
	return 0;
}


int RequestBuffer::_headers_handler()
		{
			if (_buffer.size() == 0)				return 0; // This mean that the buffer is empty.
			int _pos  = _buffer.find("\r\n", 2);
			if (_pos == -1)							return 0; // This mean that the buffer does not contain a full line. so we return.
			if (_pos == 0) // This mean that we have reached the end of the headers.
			{
				_buffer.resize(2);
				_level++;
				if (_method == "POST")
					_status = _body_handler();
				else // TEST: remove this else if you want to allow GET requests with body.
					_status = 1;
				return (_status);
			}
			while (_buffer.size() > 0 && (_pos = _buffer.find("\r\n", 2)) > 0) // this loop will run until the buffer is empty or we reach the end of the headers.
			{
				char *_tmp = nullTerminate(_buffer.substr(0, _pos), _pos);
				if (_tmp)
				{
					_headers += _tmp;
					_headers += "\n";
					delete [] _tmp;
				}
				_buffer.resize(_pos + 2);
			}
			return 0;
		}


int	RequestBuffer::_get_body_level()
{
	int _pos = _headers.find("Transfer-Encoding: ");
	if (_pos != std::string::npos)
	{
		_pos += 19;
		std::string _tmp = _headers.substr(_pos, _headers.find("\n", _pos) - _pos);
		if (_tmp == "chunked") return 2;
		return 501;
	}
	_pos = _headers.find("Content-Length: ");
	if (_pos != std::string::npos)
	{
		_pos += 16;
		std::string _tmp = _headers.substr(_pos, _headers.find("\n", _pos) - _pos);
		std::stringstream ss(_tmp);
		ss >> _contentLength;
		if (_contentLength > _maxBodySize) return 413;
		return 1;
	}
	_pos = _headers.find("Content-Type: ");
	if (_pos != std::string::npos)
	{
		_pos += 14;
		std::string _tmp = _headers.substr(_pos, _headers.find("\n", _pos) - _pos);
		if (_tmp.find("multipart/form-data") != std::string::npos)
		{
			_pos = _tmp.find("boundary=");
			if (_pos != std::string::npos)
			{
				_pos += 9;
				_boundary = _tmp.substr(_pos, _tmp.size() - _pos);
				_boundary = "--" + _boundary;
				return 3;
			}
		}
		return 501;
	}
	return (-1);
}


int RequestBuffer::_content_length_handler()
{
	if (_body_path.size() == 0)	{_body_path = _generate_tmp_file_path();}
	std::ofstream _file(_body_path, std::ios::out | std::ios::binary | std::ios::app);
	if (_file.is_open() == false)
	{
		std::cout << RED"Inernal server error: " << "failed to create a tmp file." << _body_path << std::endl;
		return (_status = 500, _status);
	}
	if (_buffer.size() >= _contentLength)
	{
		_file.write(_buffer.getData(), _contentLength);
		_file.close();
		_buffer.resize(_contentLength);
		_contentLength = 0;
		return (_status = 1, _status);
	}
	else
	{
		_file.write(_buffer.getData(), _buffer.size());
		_contentLength -= _buffer.size();
		_buffer.resize(_buffer.size());
		_file.close();
	}
	return 0;
}


std::string RequestBuffer::_generate_tmp_file_path()
{
		std::string _path;
	int i = 0;
	while (true)
	{
			_path = "/tmp/file_";
		std::stringstream ss;
		ss << i++;
		_path += ss.str();
		if (access(_path.c_str(), F_OK) != 0)
		{
				break;
		}
	}
	return _path;
}


int	RequestBuffer::_chunked_handler()
{
	if (_chunkSize == -1)
	{
		int _pos = _buffer.find("\r\n", 2);
		if (_pos == -1)						return 0; // This mean that the buffer does not contain a full line. so we return.
		char *_tmp = nullTerminate(_buffer.substr(0, _pos), _pos);
		std::stringstream ss(_tmp);
		ss >> std::hex >> _chunkSize;
		if (_tmp != NULL) delete [] _tmp;
		_buffer.resize(_pos + 2);
	}
	if (_chunkSize == 0)	return (_status = 1, _status);
	else
	{
		if (_body_path.size() == 0)	{_body_path = _generate_tmp_file_path();}
		std::ofstream _file(_body_path, std::ios::out | std::ios::binary | std::ios::app);
		if (_file.is_open() == false)
		{
			std::cout << RED"Inernal server error: " << "failed to create a tmp file." << _body_path << std::endl;
			return (_status = 500, _status);
		}
		if (_buffer.size() >= _chunkSize)
		{
			_file.write(_buffer.getData(), _chunkSize);
			_file.close();
			_buffer.resize(_chunkSize + 2);
			_chunkSize = -1;
		}
		else
		{
			_file.write(_buffer.getData(), _buffer.size());
			_chunkSize -= _buffer.size();
			_buffer.resize(_buffer.size());
			_file.close();
		}
	}
	return (0);
}


int RequestBuffer::_multipart_handler()
{
	int _pos;
	if (_multiform_data_tmp_path.size() == 0)
	{
		_multiform_data_tmp_path = _generate_tmp_file_path();
	}
	std::ofstream _file(_multiform_data_tmp_path, std::ios::out | std::ios::binary | std::ios::app);
	if (_file.is_open() == false)
	{
		std::cout << RED"Inernal server error: " << "failed to create a tmp file." << _multiform_data_tmp_path << std::endl;
		return (_status = 500, _status);
	}
	std::string _line;
	std::ifstream _file2(_multiform_data_tmp_path, std::ios::in | std::ios::binary);
	while (std::getline(_file2, _line))
	{
		if (_line == (_boundary + "--"))
		{
			_file.close();
			_file2.close();
			_buffer.resize(_buffer.size());
			return (_status = 1, _status);
		}
	}
	if ((_pos = _buffer.find((_boundary + "--").c_str(), (_boundary.size() + 2))) == -1)
	{
		_file.write(_buffer.getData(), _buffer.size());
		_buffer.resize(_buffer.size());
		_file.close();
		return (0);
	}
	else {
		_file.write(_buffer.getData(), _pos + (_boundary.size() + 2));
		_file.close();
		_buffer.resize(_pos + (_boundary.size() + 2));
		return (_status = 1, _status);
	}
	return (0);
}

int RequestBuffer::_body_handler()
{
	if (_body_level == -1)
		_body_level = _get_body_level(); // This mean that we have not yet determined the type of the body.-> 1: Content-Length, 2: chunked, 3: multipart/form-data. -1: undetermined.
	if (_body_level == -1)
		return 1; // This mean that the request does not have a body. so we return.
	else if (_body_level > 3)
		return _body_level; // This mean that the body type is not supported.
	if (_buffer.size() == 0)				return 0; // This mean that the buffer is empty.
	switch (_body_level)
	{
		case 1:
			_status = _content_length_handler();
			break;
		case 2:
			_status = _chunked_handler();
			break;
		case 3:
			_status = _multipart_handler();
			break;
		default:
			break;
	}
	return _status;
}

int	RequestBuffer::insertBuffer(const char *buffer, int size)
{
	// std::cout << buffer << " " << size<< std::endl;
	_buffer = _buffer + Byte(buffer, size);

	switch (_level)
	{
		case 0: _status = _first_line_handler(); break;
		case 1: _status = _headers_handler(); break;
		case 2: _status = _body_handler(); break;
		default: break;
	}
	return _status;
}
