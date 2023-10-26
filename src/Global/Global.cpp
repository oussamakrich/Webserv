
#include "../../include/Global.hpp"
#include <sys/poll.h>
#include <vector>

std::vector<struct pollfd> Global::gPollFds =  std::vector<struct pollfd>();

Global::Global(){}

Global::~Global(){}

Global::Global(const Global &copy){*this = copy;}

Global &Global::operator=(const Global &copy){
	servers = copy.servers;
	return *this;
}

void Global::addServer(Server *server){
	this->servers.push_back(server);
}

void  Global::print(){
		std::vector<Server *>::iterator it = servers.begin();

		for(;it != servers.end(); it++){
			std::cout << *(*it) << std::endl;
		}
};

void Global::callHandelFds(struct pollfd pfd){
	Server *server;
	
	for (unsigned int i =0; i < servers.size(); i++){
		server = servers[i];
		if (server->handelFd(pfd))
			break;
	}
}

void  Global::run()
{
	std::vector<Server *>::iterator it = servers.begin();

		for(;it != servers.end(); it++){
			std::cout << (*it)->start() << std::endl;
		std::cout << "my listen : " << (*it)->getListen() << std::endl;
		}
		
		while(true){
			std::cout << "Polling" << std::endl;
			int pollStatus = poll(this->gPollFds.data(), this->gPollFds.size(), -1);
			if (pollStatus == -1) {
				perror("poll");
				break;
			}
			for(unsigned int i =0; i < gPollFds.size(); i++){
				if ((gPollFds[i].revents & POLLIN)){
					this->callHandelFds(gPollFds[i]);
				}
			}
		}
}

void Global::insertFd(int fd){

	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT;
	Global::gPollFds.push_back(pfd);
}

