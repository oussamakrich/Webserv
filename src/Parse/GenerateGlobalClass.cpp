#include "../../include/GenerateGlobalClass.hpp"
#include <sstream>
#include <string>



Server *GenerateGlobalClass::fillServer(TOKEN_IT &it){

	Server *server = new Server();
	while(it->first != CLOSE_C_BRACKET){
		switch (it->first) {
			case LOCATION				: server->fillLocation(++it);	break;
			case TYPES					:	server->SetTypes(++it);			break;				
			case MAX_BODY_SIZE	: server->SetInt(it);					break;
			case ERROR_PAGES		: server->SetErrorPages(++it);	break;
			case INDEX					: server->SetMultiValue(it);	break;
			case ROOT						: server->SetSingleValue(it); break;
			case LISTEN					: server->SetHostAndPort(++it); break;
			case SERVER_NAME		: server->SetSingleValue(it); break;
			case DEFAULT_TYPE		: server->SetSingleValue(it); break;
			case ERROR_LOG			: server->SetSingleValue(it); break;
			case ACCESS_LOG			: server->SetSingleValue(it); break;
			case OPEN_C_BRACKET :	break;													
			case COLON					:	break;													
			case SEMICOLON			:	error("Unexpected SEMICOLONE");													
			default							: error("Unexpected key : " + it->second);
		}
		it++;
	}
	if (server->getRoot().empty())
		error("root is required");
	server->Shrink();
	return server;
} 

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
			server = fillServer(++it);
			if (checkHostAndPort(server, portAndHost)) global->addServer(server);
		}
	}
	return global;
}
