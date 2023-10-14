#pragma  once

#include "includes.hpp"


class Server {

private:
	int port;
	int maxBodySize;
	int minBodySize;
	std::string host;
	std::string serverName;


public:
	~Server();
	Server();
private:
	Server(const Server &copy);
	Server operator=(const Server &copy);



};
