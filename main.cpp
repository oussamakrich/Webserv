#include "include/includes.hpp"
#include "Parsing/include/Tokenizer.hpp"
#include "Parsing/include/GenerateGlobalClass.hpp"
#include "HttpElement/include/Cgi.hpp"

#ifndef CONFIG_PATH
# define CONFIG_PATH "./config/default.conf"
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
		std::cerr << "Usage: ./webserv [config_file]" << std::endl;
		return (1);
	}
	try
	{
		std::ifstream file(path);
		if (!file.is_open())
			throw std::runtime_error("Error: Can't open config file");
		std::vector<TOKEN> tokens = Tokenizer::tokenGenerator(file, path);

		file.close();
		Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);

		sigChange();
		WebServer->run();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
