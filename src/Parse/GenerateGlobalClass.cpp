#include "../../include/GenerateGlobalClass.hpp"

		GenerateGlobalClass::GenerateGlobalClass(){}
		GenerateGlobalClass::~GenerateGlobalClass(){}
		GenerateGlobalClass::GenerateGlobalClass(const GenerateGlobalClass &copy){*this = copy;}
		GenerateGlobalClass &GenerateGlobalClass::operator=(const GenerateGlobalClass &copy){ (void)copy; return  *this;}




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
	return server;
} 


Global *GenerateGlobalClass::generateGlobalClass(std::vector<TOKEN> tokens){
	Global *global = new Global();

	TOKEN_IT it;

	for(it = tokens.begin(); it != tokens.end(); it++){
		if (it->first == SERVER)
			global->addServer(fillServer(++it));
	}

	return global;
}
