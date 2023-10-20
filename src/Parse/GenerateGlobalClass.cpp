#include "../../include/GenerateGlobalClass.hpp"



Server *fillServer(TOKEN_IT &it){

	Server *server = new Server();
	while(it->first != CLOSE_S_BRACKET){
		switch (it->first) {
			case LOCATION				: break;
			case PORT						: server->SetPort(it);				break;
			case MAX_BODY_SIZE	: server->SetPort(it);				break;
			case ERROR_PAGES		: server->SetMultiValue(it);	break;
			case INDEX					: server->SetMultiValue(it);	break;
			default							: server->SetSingleValue(it); break;
		}
	}
	return server;
} 


Global *generateGlobalClass(std::vector<TOKEN> &tokens){
	Global *global = new Global();

	TOKEN_IT it;
	TOKEN_PAIR pair;

	for(it = tokens.begin(); it != tokens.end(); it++){
		pair = *it;
		if (pair.first == SERVER){
			//NOTE : (fillServer) fill all attribute of the server and (addServer) add that server to Servers 
			global->addServer(fillServer(it));
		}
	}

	return global;
}
