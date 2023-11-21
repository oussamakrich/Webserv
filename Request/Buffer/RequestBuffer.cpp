#include "RequestBuffer.hpp"
#include "../../HttpElement/include/Cgi.hpp"

#include <sstream>
#include <fstream>
#include <sys/_types/_size_t.h>
#include <unistd.h>

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
void										RequestBuffer::setMaxBodySize(size_t MaxBodySize) {_maxBodySize = MaxBodySize;};
void										RequestBuffer::setURI(std::string const &URI) {_uri = URI;};
void										RequestBuffer::setBody(char *body, int size) {_body_path = std::string(body, size);};


void RequestBuffer::clear()
{
	_buffer.clear();
	_headers.clear();
	_headers = "";
	_level = 0;
	_contentLength = -1;
	_status = 0;
	_chunkSize = -1;
	_boundary = "";
	_body_level = -1;
	_seek_pos = 0;
	_increment = 0;
}

RequestBuffer::RequestBuffer(size_t MaxBodySize)
{
	_increment = 0;
	_maxBodySize = MaxBodySize;
	_level = 0;
	_contentLength = -1;
	_status = 0;
	_chunkSize = -1;
	_boundary = "";
	_body_level = -1;
	_seek_pos = 0;
}

int RequestBuffer::_first_line_handler()
{
	int increment = 0;
	int _pos = _buffer.find("\r\n", 2);
	increment = 2;
	if (_pos == -1)
	{
		_pos = _buffer.find("\n", 1);
		increment = 1;
	}
	if (_pos == -1)						return 0;

	char *_tmp = nullTerminate(_buffer.substr(0, _pos), _pos);
	std::string first_line = _tmp;
	if (_tmp != NULL) delete [] _tmp;
	_buffer.resize(_pos + increment);
	_pos = 0;
	// calculate the number of space in the first line.
	for (std::string::iterator it = first_line.begin(); it != first_line.end(); it++)
		if (*it == ' ')
			_pos++;
	// if the number of space is not 2, return 400: bad request.
	if (_pos != 2)						return 400;
	std::stringstream ss(first_line);
	ss >> _method >> _uri >> _protocol; // insert first line.
	if (getMethodCode(_method) == Request::NOT_SUPPORTED)	return 501; // not implemented.
	if (getMethodCode(_method) == Request::INVALID_REQUEST)	return 400; // not implemented.
	// if the protocol is not HTTP/1.1, return 505: HTTP version not supported.
	if (_protocol != "HTTP/1.1")		return 505;
	// URI too long.
	if (_uri.size() > 2048)				return 414;
	// if the URI contains not allowed characters, return 400: bad request.
	if (_uri.find_first_not_of(ALLOWED_CHARS) != std::string::npos) return 400;
	if ((_pos = _buffer.find("\r\n", 2)) == 0 || (_pos = _buffer.find("\n", 1)) == 0)	return 1; // This mean that the request does not have headers.
	_level++;
	_status = _headers_handler();
	_found = true;
	return _status;
}


int RequestBuffer::_headers_handler()
{
	int increment = 0;
	if (_buffer.size() == 0)				return 0; // This mean that the buffer is empty.

	int _pos  = _buffer.find("\r\n", 2);
	increment = 2;
	if (_pos == -1)
	{
		_pos = _buffer.find("\n", 1);
		increment = 1;
		if (_pos == -1)						return 0; // This mean that the buffer does not contain a full line. so we return.
	}
	if (_pos == 0) // This mean that we have reached the end of the headers.
	{
		_buffer.resize(increment);
		_level++;
		if (_method == "POST")
			_status = _body_handler();
		else // TEST: remove this else if you want to allow GET requests with body.
			_status = 1;
		return (_status);
	}
	int _pos2;
	while (_buffer.size() > 0)
	{
		_pos = _buffer.find("\r\n", 2);
		_pos2 = _buffer.find("\n", 1);
		if ((_pos2 == -1 && _pos == -1) || (_pos == 0 || _pos2 == 0)) break;;
		if ((_pos2 != -1 && _pos != -1 && _pos2 < _pos) || (_pos == -1))
		{
			_pos = _pos2;
			increment = 1;
		}
		else
			increment = 2;
		char *_tmp = nullTerminate(_buffer.substr(0, _pos), _pos);
		if (_tmp)
		{
			_headers += _tmp;
			_headers += "\n";
			delete [] _tmp;
		}
		_buffer.resize(_pos + increment);
	}
	if (_pos == 0 || _pos2 == 0) // This mean that we have reached the end of the headers.
	{
		if (_pos == 0)
			increment = 2;
		else
			increment = 1;
		_buffer.resize(increment);
		_level++;
		if (_method == "POST")
			_status = _body_handler();
		else // TEST: remove this else if you want to allow GET requests with body.
			_status = 1;
		return (_status);
	}
	return 0;
}


int	RequestBuffer::_get_body_level()
{
	unsigned long _pos = _headers.find("Transfer-Encoding: ");
	if (_pos != std::string::npos)
	{
		_pos += 19;
		std::string _tmp = _headers.substr(_pos, _headers.find("\n", _pos) - _pos);
		if (_tmp == "chunked") return 2;
			return 501;
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
	}
	_pos = _headers.find("Content-Length: ");
	if (_pos != std::string::npos)
	{
		_pos += 16;
		std::string _tmp = _headers.substr(_pos, _headers.find("\n", _pos) - _pos);
		std::stringstream ss(_tmp);
		ss >> _contentLength;
		if (static_cast<size_t>(_contentLength) > _maxBodySize) return (_status = 413, _status);
		if (_contentLength == 0) return (_status = 1, _status);
		return 1;
	}
	return (-1);
}


int RequestBuffer::_content_length_handler()
{
	if (_body_path.size() == 0)	{_body_path = Cgi::getRandomName("/tmp/", "omm");}
	std::ofstream _file(_body_path, std::ios::out | std::ios::binary | std::ios::app);
	if (_file.is_open() == false)
		return (_status = 507, _status);
	if ((_buffer.size()) >= static_cast<unsigned long>(_contentLength))
	{
		_file.write(_buffer.getData(), _contentLength);
		_buffer.resize(_contentLength);
		_contentLength = 0;
		return (_status = 1, _status);
	}
	else
	{
		_file.write(_buffer.getData(), _buffer.size());
		_contentLength -= _buffer.size();
		_buffer.resize(_buffer.size());
	}
	_file.close();
	return 0;
}

int	RequestBuffer::_chunked_handler()
{
	if (_buffer.size() == 0)				return 0; // This mean that the buffer is empty.
	if (_maxBodySize <= 0)					return (unlink(_body_path.c_str()), _status = 413, _status);
	if (_chunkSize == -1)
	{
		int _pos = _buffer.find("\r\n", 2);
		_increment = 2;
		if (_pos == -1)
		{
			_pos = _buffer.find("\n", 1);
			_increment = 1;
			if (_pos == -1)						return 0; // This mean that the buffer does not contain a full line. so we return.
		}
		char *_tmp = nullTerminate(_buffer.substr(0, _pos), _pos);
		std::stringstream ss(_tmp);
		ss >> std::hex >> _chunkSize;
		if (_tmp != NULL) delete [] _tmp;
		_buffer.resize(_pos + _increment);
		if ((_pos + _increment > _maxBodySize))
			return (_maxBodySize = 0, unlink(_body_path.c_str()), _status = 413, _status);
		_maxBodySize -= _pos + _increment;
	}
	if (_chunkSize == 0)	return (_status = 1, _status);
	else
	{
		if (_body_path.size() == 0)	{_body_path = Cgi::getRandomName("/tmp/", "omm");}
		std::ofstream _file(_body_path, std::ios::out | std::ios::binary | std::ios::app);
		if (_file.is_open() == false)
		{
			return (_status = 507, _status);
		}
		if (static_cast<int>(_buffer.size()) >= _chunkSize)
		{
			_file.write(_buffer.getData(), _chunkSize);
			_file.close();
			_buffer.resize(_chunkSize + _increment);
			_chunkSize = -1;
		}
		else
		{
			_file.write(_buffer.getData(), _buffer.size());
			_chunkSize -= _buffer.size();
			_buffer.resize(_buffer.size());
			_file.close();
		}
		return (_chunked_handler());
	}
	return (0);
}


int RequestBuffer::_multipart_handler()
{
	int _pos;
	if (_body_path.size() == 0)
		_body_path = Cgi::getRandomName("/tmp/", "_file");
	if (_maxBodySize <= 0)
		return (unlink(_body_path.c_str()), _status = 413, _status);

	std::ofstream _file(_body_path, std::ios::out | std::ios::binary |
	std::ios::app);
	if (_file.is_open() == false)
		return (_status = 507, _status);
	std::string _line;
	std::ifstream _file2(_body_path, std::ios::in | std::ios::binary);
	_file2.seekg(_seek_pos);
	while (std::getline(_file2, _line))
	{
		if (_line == (_boundary + "--"))
		{
			_file.close();
			_file2.close();
			delete [] _buffer._data;
			_buffer._size = 0;
			return (_status = 1, _status);
		}
	}
	_seek_pos = _file2.tellg();
	if ((_pos = _buffer.find((_boundary + "--").c_str(), (_boundary.size() + 2)))
	== -1)
	{
		if (_buffer.size() > _maxBodySize)
		{
			_file.close();
			_file2.close();
			return (unlink(_body_path.c_str()), _status = 413, _status);
		}
		_maxBodySize -= _buffer.size();
		_file.write(_buffer.getData(), _buffer.size());
		_buffer.resize(_buffer.size());
		_file.close();
		_file2.close();
		return (0);
	}
	else
	{
		_file.write(_buffer.getData(), _pos + (_boundary.size() + 2));
		_file.close();
		_file2.close();
		delete []_buffer._data;
		_buffer._size = 0;
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
	if (_buffer.size() == 0)				return _status; // This mean that the buffer is empty.
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
