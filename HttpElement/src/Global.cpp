
#include "../include/Global.hpp"
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

void Global::removeFd(int fd){
	for(unsigned int i=0; i < gPollFds.size(); i++){
			if (gPollFds[i].fd == fd){
				gPollFds.erase(gPollFds.begin() + i);
				break ;
		}
	}
}

void checkTimeOut(vector<Server*> &servers){
	vector<Server*>::iterator it  = servers.begin();
	for(;it != servers.end(); it++){
		(*it)->checkTimeOut();
	}
}

void  Global::run()
{
	std::vector<Server *>::iterator it = servers.begin();

	for(;it != servers.end(); it++){
		if (!(*it)->start()){
			delete *it;
			servers.erase(it);
		}
		if (servers.size() == 0)
			exit(1);
	}
	while(true){
		// checkTimeOut(servers);
		// std::cout << "size of gPollFds : " << Global::gPollFds.size() << std::endl;

		int pollStatus = poll(this->gPollFds.data(), this->gPollFds.size(), -1);
		if (pollStatus == -1) {
			perror("poll");
			break;
		}
		for(unsigned int i =0; i < gPollFds.size() && pollStatus; i++){


			if ((gPollFds[i].revents & POLLIN)){
				char buf[1024];
				// while (read(gPollFds[i].fd, buf, 1024) > 0);
				this->callHandelFds(gPollFds[i]);
				pollStatus--;
			}
		}
	}
}

void Global::insertFd(int fd){

	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	Global::gPollFds.push_back(pfd);
}
