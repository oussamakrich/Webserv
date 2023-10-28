#pragma once

#include "../../include/includes.hpp"


class RequestBuffer;

class Request;

class Client{


	private:
		pollfd	pfd;
		long		lastTime;
		int			status;

		RequestBuffer reqBuff;

	public: //Utils
		void		ReadRequest();
		bool		isRequestAvailable();
		Request getRequest();

	public: //Geters
		int getFd();

	public: //Seters
		void setFd(int fd);

};
