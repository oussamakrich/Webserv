#pragma once

#include "../../include/includes.hpp"
#include "../../Parsing/include/ParsRequest.hpp"
#include "../../Response/include/Response.hpp"


#define ITT_CLIENT	std::vector<Client*>::iterator

class Server;


class Client{

	public :
		Client(int bodySize, int fd);
		~Client();
		int					fd;
		std::string			id;
		long long				time;

	private:
		std::time_t			lastTime;
		int							status;
		std::string			address;
		sockaddr			sockaddr;

	public:
		RequestBuffer reqBuff;
		Response *response;
		bool			IhaveResponse;
		bool			IhaveCGI;
		bool			keepAlive;
		bool			CGIFinish;
		bool		IhaveUpload;

	public: //Utils
		bool		ReadRequest();
		bool		isRequestAvailable();
		Request *getRequest();
		std::time_t	getLastTime();

		bool NewRequest(Server &ser);
		bool OldRequest();
		bool CgiRequest();

		void ClientUpload(Server &ser);
		void Error();

	public: //Geters
		int getFd();
		struct sockaddr &getAddr();
		void setAddr(struct sockaddr &addr);
		void setLastTime(std::time_t tm);
		void switchEvent(int fd, int Flag);
		void resetClient();
		void clearClient();

};
