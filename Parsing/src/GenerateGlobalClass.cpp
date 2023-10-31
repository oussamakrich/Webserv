#include "../include/GenerateGlobalClass.hpp"
#include "../include/GenerateServer.hpp"


void warning(std::string warn){
	std::cout << YELLOW"WARNING : " << RESET << warn << std::endl;
}

bool	GenerateGlobalClass::checkHostAndPort(Server *server, std::vector<HOST_PORT> &vect){
	HOST_PORT pair;

	pair.first = server->getPort();
	pair.second = server->getHost();

	std::stringstream ss;
	ss << "Server Duplicate : "  << pair.first << " and "+ pair.second;

	if (std::find(vect.begin(), vect.end(), pair) != vect.end()){
		warning(ss.str());
		return false;
	}
	vect.push_back(pair);
	return true;
}


Global *GenerateGlobalClass::generateGlobalClass(std::vector<TOKEN> tokens){
	Global *global = new Global();
	Server *server;
	std::vector<HOST_PORT> portAndHost;

	TOKEN_IT it;

	for(it = tokens.begin(); it != tokens.end(); it++){
		if (it->first == SERVER){
			server = GenerateServer::NewServer(++it);
			if (checkHostAndPort(server, portAndHost)) global->addServer(server);
		}
	}
	return global;
}
