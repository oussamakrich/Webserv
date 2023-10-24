#include "../../include/includes.hpp"
#include "../../include/Server.hpp"
#include "../../include/Global.hpp"
#include <map>

void printEnam(Token t)
{
	if (t == RETURN)
		std::cout << "RETURN";
	else if (t == LOCATION)
		std::cout << "LOCATION";
	else if (t == WORD)
		std::cout << "WORD";
	else if (t == SERVER)
		std::cout << "SERVER";
	else if (t == OPEN_S_BRACKET)
		std::cout << "OPEN_S_BRACKET";
	else if (t == CLOSE_S_BRACKET)
		std::cout << "CLOSE_S_BRACKET";
	else if (t == OPEN_C_BRACKET)
		std::cout << "OPEN_C_BRACKET";
	else if (t == CLOSE_C_BRACKET)
		std::cout << "CLOSE_C_BRACKET";
	else if (t == SEMICOLON)
		std::cout << "SEMICOLON";
	else if (t == INCLUDE)
		std::cout << "INCLUDE";
	else if (t == ALLOWED_METHOD)
		std::cout << "ALLOWED_METHOD";
	else if (t == COLON)
		std::cout << "COLON";
	else if (t == TYPES)
		std::cout << "TYPES";
	else if (t == SPACE)
		std::cout << "SPACE";
	else if (t == LISTEN)
		std::cout << "LISTEN";
	else if (t == ROOT)
		std::cout << "ROOT";
	else if (t == SERVER_NAME)
		std::cout << "SERVER_NAME";
	else if (t == INDEX)
		std::cout << "INDEX";
	else if (t == MAX_BODY_SIZE)
		std::cout << "MAX_BODY_SIZE";
	else if (t == ERROR_PAGES)
		std::cout << "ERROR_PAGES";
	else if (t == DEFAULT_TYPE)
		std::cout << "DEFAULT_TYPE";
	else if (t == ERROR_LOG)
		std::cout << "ERROR_LOG";
	else if (t == ACCESS_LOG)
		std::cout << "ACCESS_LOG";
	else if (t == TRY_FILES)
		std::cout << "TRY_FILES";
	else if (t == REWRITE)
		std::cout << "REWRITE";
	else if (t == AUTO_INDEX)
		std::cout << "AUTO_INDEX";
	else
		std::cout << "UNKNOWN";

}

void printTokens(std::vector<TOKEN> tokens)
{
	for (std::vector<TOKEN>::iterator it = tokens.begin(); it != tokens.end(); it++)
		{
			std::cout << RED"Token: ";
			std::cout << RESET;
			printEnam(it->first);
			std::cout << GREEN"\tValue: ";
			std::cout << RESET;
			std::cout  << it->second  << std::endl;
		}
}

void printPages(std::vector<ERRPAGE_PAIR> vect){
std::vector<ERRPAGE_PAIR>::iterator it = vect.begin();
	for(;it != vect.end(); it++){
		std::cout << "|\t" << it->first << "	:	" << it->second << "\t |" << std::endl;
	}
}

void printvect(std::vector<std::string> vect){
std::vector<std::string>::iterator it = vect.begin();
	for(;it != vect.end(); it++){
		std::cout << *it;
		std::cout << "  ";
	}
	std::cout <<std::endl;
}
void printmap(std::map<std::string, std::string> vect){
std::map<std::string,std::string >::iterator it = vect.begin();
	for(;it != vect.end(); it++){
			std::cout << "|\t" << it->first << "	:	" << it->second << "\t |"<< std::endl;
	}
	std::cout << "++++++++++++++++++++++++++++++++++++" <<std::endl;

}

void Server::print(){
	std::cout << "++++++++++++serverName : " << serverName <<"++++++++++++" << std::endl;
	std::cout << "| port			: " << port << std::endl;
	std::cout << "| host			: " << host << std::endl;
	std::cout << "| body			: " << clientMaxBodySize << std::endl;
	std::cout << "| root			: " << root << std::endl;
	std::cout << "| index			: "; printvect(index);
	std::cout << "| deflttype		: " << defaultType << std::endl;
	std::cout << "| accesslog		: " << accessLog << std::endl;
	std::cout << "| errorLog		: " << errorLog << std::endl;
	std::cout << "|----------------errorpage---------------" <<std::endl; printPages(errorPages);
	std::cout << "|----------------mimetype----------------" << std::endl; printmap(mimeType);
	std::cout << "|----------------location----------------" << std::endl;
	std::map<std::string,Location*>::iterator it = locations.begin();

	for(;it != locations.end(); it++){
			std::cout << "|\t URL : "  << it->first << std::endl;
			it->second->printLocation();
			std::cout << "|"<< std::endl;
	}
	std::cout << "++++++++++++++++++++++++++++++++++++" <<std::endl;

}


