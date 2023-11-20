#include "include/includes.hpp"
#include "Parsing/include/Tokenizer.hpp"
#include "Parsing/include/GenerateGlobalClass.hpp"
#include "HttpElement/include/Cgi.hpp"
#ifndef CONFIG_PATH
# define CONFIG_PATH "./config/defaultff.conf"
#endif
int main (int argc, char **argv)
{
	std::cout << CONFIG_PATH << std::endl;
	std::string path = argc == 1 ? "./config/default.conf" : argv[1];
	try
	{
		std::ifstream file(argv[1]);
		std::vector<TOKEN> tokens = Tokenizer::tokenGenerator(file);
		Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);

		sigChange();
		WebServer->run();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}
