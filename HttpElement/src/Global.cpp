
#include "../include/Global.hpp"
#include <sys/poll.h>
std::vector<struct pollfd> Global::gPollFds =  std::vector<struct pollfd>();
std::vector<std::string> Global::serverNames =  std::vector<std::string>();
std::vector<Server *> Global::servers =  std::vector<Server *>();

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

	for (unsigned int i =0; i < servers.size(); i++)
	{
		if (servers[i]->handelFd(pfd))
			break;
	}
}

void Global::removeFd(int fd){
	for(unsigned int i=0; i < gPollFds.size(); i++){
			if (gPollFds[i].fd == fd){
				close(gPollFds[i].fd);
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

	for(;it != servers.end(); it++) {
		if (!(*it)->start()){
			delete *it;
			servers.erase(it);
		}
		if (servers.size() == 0)
			exit(1);
		if (!(*it)->getServerName().empty())
			serverNames.push_back((*it)->getServerName());
	}
	while(true){
		checkTimeOut(servers);
		std::cout << "try pool\n";

		int pollStatus = poll(this->gPollFds.data(), this->gPollFds.size(), 1);
		std::cout << "after try pool\n";
		if (pollStatus == -1)
		{
			std::cerr << "POLL FAILED" << std::endl;
			continue;
		}
		for( int i = gPollFds.size() - 1 ; i >= 0; i--){
			std::cout << "index " << i << std::endl;;
			if (pollStatus && ((gPollFds[i].revents & POLLIN) || (gPollFds[i].revents & POLLOUT))){
				this->callHandelFds(gPollFds[i]);
				pollStatus--;
			}
		}
	}
}


/*
for(unsigned int i =0; i < gPollFds.size(); i++){
			if (pollStatus && ((gPollFds[i].revents & POLLIN) || (gPollFds[i].revents & POLLOUT))){
				this->callHandelFds(gPollFds[i]);
				pollStatus--;
			}
		}
*/

Server &Global::FindServer(const MAP_STRING &headers, Server &ser){

	std::string value;

	value = headers.at("Host");
	if (value.empty())
		return ser;
	stringstream ss(value);
	std::getline(ss, value, ':');

	std::vector<std::string>::iterator it;

	it = std::find(serverNames.begin(), serverNames.end(), value);
	if (it == serverNames.end() || value == ser.getServerName())
		return ser;

	size_t size = servers.size();
	for (size_t i = 0;i < size; i++){
		if (servers[i]->getServerName() == value && servers[i]->getPort() == ser.getPort() && servers[i]->getHost() == ser.getHost())
			return *servers[i];
	}
	return ser;
}

void Global::switchEvent(int fd, int Flag){
	for (unsigned int i = 0; i < gPollFds.size(); i++){
		if (gPollFds[i].fd == fd){
			gPollFds[i].events = Flag;
			break ;
		}
	}
}

void Global::insertFd(int fd){

	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	Global::gPollFds.push_back(pfd);
}


