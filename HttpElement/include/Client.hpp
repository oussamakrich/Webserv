#pragma once

#include "../../include/includes.hpp"

class Client{

	public:
		int getFd();

	public:
		void setFd(int fd);

	private:
		struct pollfd pfd;
};
