#include "include/includes.hpp"
#include "Parsing/include/Tokenizer.hpp"
#include "Parsing/include/GenerateGlobalClass.hpp"

void ff(void)
{
	system("leaks webserver");
}


int main (int argc, char **argv)
{
	// atexit(ff);
	if (argc != 2){
		std::cerr << "Error: required a config file" << std::endl;
		return 1;
	}
	try
	{
		std::ifstream file(argv[1]);

		std::vector<TOKEN> tokens = Tokenizer::tokenGenerator(file);//


		// printTokens(tokens);

		Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);
		// WebServer->print();

		WebServer->run();

	}catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	// int x;cin >> x;

	return (0);
}

