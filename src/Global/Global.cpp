#include "../../include/Global.hpp"

Global::Global(){

}
Global::~Global(){}

Global::Global(const Global &copy){*this = copy;}

Global &Global::operator=(const Global &copy){
	servers = copy.servers;
	mime_types = copy.mime_types;
	return *this;
}





void Global::addServer(Server *server){
	this->servers.push_back(server);
}
