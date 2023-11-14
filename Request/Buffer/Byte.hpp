#pragma once
#include <iostream>
#include <string>

class Byte{

	public:
		size_t _size;
		char *_data;
	public:

		int operator!();

		friend const Byte &operator+(Byte &lhb, const Byte &rhb);

		char operator[](size_t _i) {return _data[_i];}

		Byte(const char *_data, size_t _size);

		Byte(const char *lhs, const char *rhs, size_t lhs_size, size_t rhs_size);

		~Byte();

		int find(const char *_to_find, int _len) const;

		char *substr(int start, int len) const;

		char *substr(int start);

		void resize(size_t _start);

		size_t size() const;


		Byte();

		void setData(const char *_data, size_t _size);
		bool operator==(const Byte &lhs) const;

		char *getData() const {return _data;}

		friend std::ostream &operator<<(std::ostream &out, Byte &b);

		void	clear();

};

char *strdup(const char *s, int size);


char *strjoin(const char *lhs, const char *rhs, int lhs_size, int rhs_size);
char *nullTerminate(char *s, int size);
