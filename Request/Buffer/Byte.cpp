#include "Byte.hpp"
#include <sys/_types/_size_t.h>


char *strdup(const char *s, int size)
{
	char *d = NULL;
	if (size == 0 || s == NULL) return NULL;
	try {
		d = new char[size];
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return NULL;
	}
	if (d == NULL) return NULL;
	std::memmove(d, s, size);
	return d;
}

char *strjoin(const char *lhs, const char *rhs, int lhs_size, int rhs_size)
{
	char *ret = NULL;
	try {
		ret = new char[lhs_size + rhs_size];
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return NULL;
	}
	if (lhs_size > 0)
		std::memmove(ret, lhs, lhs_size);
	if (rhs_size > 0)
		std::memmove(ret + lhs_size, rhs, rhs_size);
	return ret;
}

char *nullTerminate(char *s, int size)
{
	char *ret = NULL;
	try {
		ret = new char[size + 1];
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return NULL;
	}
	if (size > 0)
		std::memmove(ret, s, size);
	ret[size] = '\0';
	delete [] s;
	return ret;
}

int Byte::operator!()
{
	return this->find("\r\n\r\n", 4);
}

Byte::Byte(const char *_data, size_t _size)
{
	this->_data = strdup(_data, _size);
	this->_size = _size;
}

Byte::Byte(const char *lhs, const char *rhs, size_t lhs_size, size_t rhs_size)
{
	this->_data = strjoin(lhs, rhs, lhs_size, rhs_size);
	this->_size = lhs_size + rhs_size;
}

Byte::~Byte()
{
	if (_data != NULL && _size > 0)
		delete [] _data;
	_size = 0;
}

int Byte::find(const char *_to_find, size_t _len) const
{
	size_t		i;
	size_t		j;
	i = 0;
	j = 0;
	while (i < _size)
	{
		if (_data[i] == _to_find[j])
		{
			while ((i + j) < _size && _data[i + j] == _to_find[j] && j < _len)
				j++;
			if (j == _len)
				return (i);
			j = 0;
		}
		i++;
	}
	return (-1);
}

char *Byte::substr(size_t start, size_t len) const
{
	if (start < 0 || start > _size || len <= 0 || len > _size - start)
		return NULL;
	char *ret = NULL;
	try {
		ret = new char[len];
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return NULL;
	}
	std::memmove(ret, _data + start, len);
	return ret;
}

char *Byte::substr(size_t start)
{
	if (start < 0 || start > _size || _size - start <= 0)
		return NULL;
	char *ret = NULL;
	try {
		ret = new char[_size - start];
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return NULL;
	}
	std::memmove(ret, _data + start, _size - start);
	return ret;
}


void Byte::resize(size_t _start)
{
	if (_start < 0 || _start >= _size)
	{
		if (_data != NULL && _size > 0)
			delete [] _data;
		_data = NULL;
		_size = 0;
		return ;
	}
	char *tmp = _data;
	_data = substr(_start);
	if (_data == NULL)
	{
		_data = tmp;
		return ;
	}
	_size -= _start;
	delete [] tmp;
}

size_t Byte::size() const {return _size;}

Byte::Byte() {_data = NULL; _size = 0;}

void Byte::setData(const char *_data, size_t _size)
{
	if (this->_data != NULL && this->_size > 0)
		delete [] this->_data;
	this->_data = strdup(_data, _size);
	this->_size = _size;
}

bool Byte::operator==(const Byte &lhs) const
{
	if (this->_size != lhs._size)
		return false;
	for (size_t i = 0; i < this->_size; i++)
		if (this->_data[i] != lhs._data[i])
			return false;
	return true;
}

const Byte &operator+(Byte &lhb, const Byte &rhb)
{
	const char *tmp = lhb._data;
	lhb._data = strjoin(lhb._data, rhb._data, lhb._size, rhb._size);
	lhb._size += rhb._size;
	delete [] tmp;
	return lhb;
}

std::ostream &operator<<(std::ostream &out, Byte &b)
{
	for (size_t i = 0; i < b.size(); i++)
		out << b[i];
	return out;
}

void	Byte::clear()
{
	if (_data != NULL && _size > 0)
		delete [] _data;
	_data = NULL;
	_size = 0;
}
