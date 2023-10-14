
#include "../../include/ParseConfig.hpp"
#include "../../include/TokenConfig.hpp"



void ParseConfig::SyntaxError(std::string &fileString){

}

//NOTE::just for print
// void print(token tk){
// 	if (tk == ESP)
// 		std::cout << "espace" << std::endl;
// 	if (tk == SERVER)
// 		std::cout << "server" << std::endl;
// 	if (tk == SEMICOLONE)
// 		std::cout << "SEMICOLONE" << std::endl;
// 	if (tk == COLONE)
// 		std::cout << "COLONE" << std::endl;
// 	if (tk == BRACKETOPEN)
// 		std::cout << "BRACKETOPEN" << std::endl;
// 	if (tk == BRACKETCLOSE)
// 		std::cout << "BRACKETCLOSE" << std::endl;
// 	if (tk == VALUE)
// 		std::cout << "VALUE" << std::endl;
// 	if (tk == INCLUDE)
// 		std::cout << "INCLUDE" << std::endl;
// }

ParseConfig::ParseConfig(std::string path){

	std::string fileString;
	std::ifstream configFile(path);
	TokenConfig TokenConfig;
	
	if (!configFile.is_open())	{
		throw std::runtime_error("ERR_OPEN");
	}

	this->tokens = TokenConfig.TokenTheConfig(configFile);

	//NOTE:: Just For Test
	// for (std::vector<TOKEN_PAIR>::iterator it = tokens.begin(); it != tokens.end(); ++it){
	// 	TOKEN_PAIR pair = *it;
	// 	std::cout << pair.first << "   ";
	// 	print(pair.second);
	// }

	configFile.close();
}
