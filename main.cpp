#include "include/includes.hpp"
#include "Parsing/include/Tokenizer.hpp"
#include "Parsing/include/GenerateGlobalClass.hpp"
#include "HttpElement/include/Cgi.hpp"

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
	// t_cgiInfo info  = Cgi::Run(req, bin , path);
	//
	// 	int status;
	// 	while (Cgi::isFinished(info, status) == false )
	// 	{
	// 		std::cout << "wait child for finish...\n";
	// 		sleep(1);
	// 	}
	// 	std::cout << "status " << status;

	// 	// printTokens(tokens);

		Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);
		// WebServer->print();

		 WebServer->run();

	}catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	// // int x;cin >> x;

	return (0);
}

