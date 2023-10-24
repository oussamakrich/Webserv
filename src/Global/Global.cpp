
#include "../../include/Global.hpp"


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
void  Global::run()
{
	std::vector<Server *>::iterator it = servers.begin();

		for(;it != servers.end(); it++){
			std::cout << (*it)->start() << std::endl;
		}

}
