#include "include/includes.hpp"
#include "Parsing/include/Tokenizer.hpp"
#include "Parsing/include/GenerateGlobalClass.hpp"
#include "HttpElement/include/Cgi.hpp"

#ifndef CONFIG_PATH
# define CONFIG_PATH "./config/defaultff.conf"
#endif

int main (int argc, char **argv)
{
	std::string path;

	if (argc == 1)
		path = CONFIG_PATH;
	else if (argc == 2)
		path = argv[1];
	else
	{
		std::cout << "Usage: ./webserv [config_file]" << std::endl;
		return (1);
	}
	try
	{
		std::ifstream file(path);
		std::vector<TOKEN> tokens = Tokenizer::tokenGenerator(file, path);
		Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);
		// WebServer->print();

		sigChange();
		WebServer->run();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}
