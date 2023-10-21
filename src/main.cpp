#include "../include/includes.hpp"
#include "../include/Tokenizer.hpp"
#include "../include/GenerateGlobalClass.hpp"
#include <vector>


int main (int argc, char **argv)
{
	(void)argc;
	try
	{
		std::ifstream file(argv[1]);

		std::vector<TOKEN> tokens = Tokenizer::tokenGenerator(file);//

		// printTokens(tokens);
		// Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);
		//WebServer.run();

	}catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}

