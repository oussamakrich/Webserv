#include "../include/includes.hpp"
#include "../include/Tokenizer.hpp"
#include "../include/GenerateGlobalClass.hpp"

int main (int argc, char **argv)
{
	try
	{
		std::ifstream file(argv[1]);

		std::vector<TOKEN> tokens = Tokenizer::tokenGenerator(file);

		Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);
		//WebServer.run();

	}catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}


