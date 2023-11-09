#include "../include/GenerateGlobalClass.hpp"
#include "../include/GenerateServer.hpp"
#include <sys/_types/_size_t.h>


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
		return false;
	}
	vect.push_back(pair);
	return true;
}

//TODO : store listen && server_name for check repetition
// add flag if listen repeated for not start the server


void checkservername(std::vector<Server *> &ser, Server *server){
		int port = server->getPort();
		std::string host = server->getHost();
		std::string serverName = server->getServerName();

	for (size_t i =0; i < ser.size(); i++){
		if (ser[i]->getServerName() == serverName && ser[i]->getPort() == port && ser[i]->getHost() == host){
			if (!server->getServerName().empty())
				warning("Server : " + server->getServerName() + " is repeated");
			else
				warning("Server : " + server->getHost() + ":" + convertCode(server->getPort()) + " is repeated");
		}
	}
}

Global *GenerateGlobalClass::generateGlobalClass(std::vector<TOKEN> tokens){
	Global *global = new Global();
	Server *server;
	std::vector<HOST_PORT> portAndHost;

	TOKEN_IT it;

	for(it = tokens.begin(); it != tokens.end(); it++){
		if (it->first == SERVER){
			server = GenerateServer::NewServer(++it);
			if (!checkHostAndPort(server, portAndHost)){
				server->listenRepeat = true;
				checkservername(Global::servers, server);
			}
			if (server->ServerOff)
				delete server;
			else
				global->addServer(server);
		}
	}
	return global;
}
