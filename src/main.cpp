#include "../include/includes.hpp"
#include "../include/Tokenizer.hpp"
#include "../include/GenerateGlobalClass.hpp"

int main (int argc, char **argv)
{
	if (argc != 2){
		std::cerr << "Error: required a config file" << std::endl;
		return 1;
	}
	try
	{
		std::ifstream file(argv[1]);

		std::vector<TOKEN> tokens = Tokenizer::tokenGenerator(file);//


		printTokens(tokens);

		Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);
		WebServer->print();

		// WebServer.run();

	}catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}

