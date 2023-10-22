#include "../../include/includes.hpp"

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
