
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
		cout << "\t\t ================ Wait For new Evnet ================ SIZE " << gPollFds.size() << " \n ";
		int pollStatus = poll(this->gPollFds.data(), this->gPollFds.size(),0);
		if (pollStatus == -1) 
		{
			std::cout << "POLL : FILA\n";
			continue;
		}
		for(unsigned int i =0; i < gPollFds.size(); i++){
			if (pollStatus && ((gPollFds[i].revents & POLLIN) || (gPollFds[i].revents & POLLOUT))){
			
				this->callHandelFds(gPollFds[i]);
				pollStatus--;
			}
			else if (gPollFds[i].revents  != 0)
			{
				std::cout << "\n\nERRR______________________________________________________GOGO\n\n";
			}
		}
	}
}

void Global::switchEvent(int fd, int Flag){
	for (unsigned int i = 0; i < gPollFds.size(); i++){
		if (gPollFds[i].fd == fd){
			gPollFds[i].events = Flag;
			break ;
		}
	}
}

pollfd &Global::insertFd(int fd){

	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	Global::gPollFds.push_back(pfd);
	return Global::gPollFds.back();
}


