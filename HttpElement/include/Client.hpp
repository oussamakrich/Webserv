#pragma once

#include "../../include/includes.hpp"
#include "../../Parsing/include/ParsRequest.hpp"
#include "../../Response/include/Response.hpp"




class Client{

	public :
		Client(int bodySize, int fd);
		~Client();

	private:
		pollfd					pfd;
		std::time_t			lastTime;
		int							status;
		std::string			address;
		sockaddr			sockaddr;

	public:
		RequestBuffer reqBuff;
		Response *response;
		bool			IhaveResponse;

	public: //Utils
		void		ReadRequest();
		bool		isRequestAvailable();
		Request *getRequest();
		std::time_t	getLastTime();

	public: //Geters
		int getFd();
		struct sockaddr &getAddr();
		void setAddr(struct sockaddr &addr);
		void setLastTime(std::time_t tm);
		void switchEvent();

};
