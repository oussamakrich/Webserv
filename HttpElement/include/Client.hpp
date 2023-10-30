#pragma once

#include "../../include/includes.hpp"
#include "../../Request/RequestBuffer.hpp"


class Request{
	public:
		int tmp;
};

class Client{

	public :
		Client(int bodySize, int fd);

	private:
		pollfd					pfd;
		long						lastTime;
		int							status;
		std::string			address;
		sockaddr			sockaddr;

		// RequestBuffer reqBuff;

	public: //Utils
		void		ReadRequest();
		bool		isRequestAvailable();
		Request getRequest();

	public: //Geters
		int getFd();
		struct sockaddr &getAddr();
		void setAddr(struct sockaddr &addr);


};
