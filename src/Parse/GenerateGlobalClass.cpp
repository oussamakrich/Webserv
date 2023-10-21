#include "../../include/GenerateGlobalClass.hpp"

		GenerateGlobalClass::GenerateGlobalClass(){}
		GenerateGlobalClass::~GenerateGlobalClass(){}
		GenerateGlobalClass::GenerateGlobalClass(const GenerateGlobalClass &copy){*this = copy;}
		GenerateGlobalClass &GenerateGlobalClass::operator=(const GenerateGlobalClass &copy){return  *this;}




Server *GenerateGlobalClass::fillServer(TOKEN_IT &it){

	Server *server = new Server();
	while(it->first != CLOSE_S_BRACKET){
		switch (it->first) {
			case LOCATION				: break;
			case TYPES					:	server->SetTypes(++it);		break;				
			case PORT						: server->SetInt(it);				break;
			case MAX_BODY_SIZE	: server->SetInt(it);				break;
			case ERROR_PAGES		: server->SetMultiValue(it);	break;
			case INDEX					: server->SetMultiValue(it);	break;
			case HOST						: server->SetSingleValue(it); break;
			case ROOT						: server->SetSingleValue(it); break;
			case SERVER_NAME		: server->SetSingleValue(it); break;
			case DEFAULT_TYPE		: server->SetSingleValue(it); break;
			case ERROR_LOG			: server->SetSingleValue(it); break;
			case ACCESS_LOG			: server->SetSingleValue(it); break;
			default							: break;
		}
		it++;
	}
	return server;
} 


Global *GenerateGlobalClass::generateGlobalClass(std::vector<TOKEN> tokens){
	Global *global = new Global();

	TOKEN_IT it;

	for(it = tokens.begin(); it != tokens.end(); it++){
		if (it->first == INCLUDE){
			global->HandelMimeTypes(++it);
		}
		if (it->first == SERVER)
			global->addServer(fillServer(++it));
	}

	return global;
}
