#include "../../include/GenerateGlobalClass.hpp"

		GenerateGlobalClass::GenerateGlobalClass(){}
		GenerateGlobalClass::~GenerateGlobalClass(){}
		GenerateGlobalClass::GenerateGlobalClass(const GenerateGlobalClass &copy){*this = copy;}
		GenerateGlobalClass &GenerateGlobalClass::operator=(const GenerateGlobalClass &copy){return  *this;}




Server *GenerateGlobalClass::fillServer(TOKEN_IT &it){

	Server *server = new Server();
	while(it->first != CLOSE_S_BRACKET){
		switch (it->first) {
			case SERVER | OPEN_S_BRACKET : break;
			case LOCATION				: break;
			case TYPES					:	server->SetTypes(++it);		break;				
			case PORT						: server->SetPort(it);				break;
			case MAX_BODY_SIZE	: server->SetPort(it);				break;
			case ERROR_PAGES		: server->SetMultiValue(it);	break;
			case INDEX					: server->SetMultiValue(it);	break;
			default							: server->SetSingleValue(it); break;
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
