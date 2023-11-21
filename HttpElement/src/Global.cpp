#include "../include/Global.hpp"

std::vector<struct pollfd> Global::gPollFds =  std::vector<struct pollfd>();
std::vector<std::string> Global::serverNames =  std::vector<std::string>();
std::vector<Server *> Global::servers =  std::vector<Server *>();

Global::Global(){}

Global::~Global(){

	std::vector<Server *>::iterator it = servers.begin();
	for (; it != servers.end(); it++)
		delete *it;
	servers.clear();
	std::vector<struct pollfd>::iterator itFds =  gPollFds.begin();
	for (; itFds != gPollFds.end(); itFds++)
		close(itFds->fd);
	gPollFds.clear();
}

void Global::addServer(Server *server){
	this->servers.push_back(server);
}

void Global::callHandelFds(struct pollfd pfd){

	for (size_t i =0; i < servers.size(); i++)
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

void Global::startServers(){
	size_t i = 0;
	Server *server;

	while (i < servers.size()){
		server = servers[i];
		if (!server->start()){
			warning("error While starting server : " + server->getHost() +":" + convertCode(server->getPort()) + " " + server->getServerName());
			delete server;
			servers.erase(servers.begin() + i);
			if (servers.size() == 0)
				exit(1);
			continue;
		}
		if (!server->getServerName().empty())
			serverNames.push_back(server->getServerName());
		i++;
	}
}

void  Global::run(){
	int pollStatus;
	size_t i;
	int timeout = -1;

	startServers();
	while(true)
	{
		try{
			checkTimeOut(servers);
			if (gPollFds.size() != servers.size())
				timeout = TIME_OUT * MILISECONDS;
			else
				timeout = -1;
			pollStatus = poll(this->gPollFds.data(), this->gPollFds.size(), timeout);
			if (pollStatus == -1)
			{
				perror("poll");
				continue;
			}
			for (i = 0; i < gPollFds.size() && pollStatus ; i++)
			{
				if (gPollFds[i].revents & POLLHUP){
					this->callHandelFds(gPollFds[i]);
					pollStatus--;
					continue;
				}
				if (((gPollFds[i].revents & POLLIN) || (gPollFds[i].revents & POLLOUT))){
					this->callHandelFds(gPollFds[i]);
					pollStatus--;
				}
			}
		}
		catch (std::exception &e){
			std::cerr << e.what() << std::endl;	
		}
	}
}

Server &Global::FindServer(const MAP_STRING &headers, Server &ser){

	std::string value;

	try{
		value = headers.at("Host");
	}
	catch(...){
		return ser;
	}
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

void Global::insertFd(int fd) {
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	Global::gPollFds.push_back(pfd);
}


