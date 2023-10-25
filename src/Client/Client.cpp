
#include "../../include/Client.hpp"


int Client::getFd(){
	return pfd.fd;
}

void Client::setFd(int fd){
	pfd.fd = fd;
	pfd.events = POLLIN | POLLOUT;

}
