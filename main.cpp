#include "include/includes.hpp"
#include "Parsing/include/Tokenizer.hpp"
#include "Parsing/include/GenerateGlobalClass.hpp"
#include "HttpElement/include/Cgi.hpp"

void ff(void)
{
	system("leaks webserver");
}


int main (int argc, char **argv)
{


		Request req(Request::GET);
		req.setBodyBuff("hi");
		req.setBodySize(2);
		req.insertHeader("host", "test");
		req.insertHeader("Port", "test");
		req.insertHeader("Content-type", "");
		req.setQuery("name=dataddffdsfdsf");
		req.setContentLength(0);
		req.setTransferEncoding("shunked");
		req.insertHeader("Protoco", "test");
		string bin = "./cgi-bin/php-cgi";
		string path = "/Users/ebennamr/Desktop/WebServer/index.php";
		Cgi::Run(req, bin , path);

	// out << "HIE";
// atexit(ff);
	// if (argc != 2){
	// 	std::cerr << "Error: required a config file" << std::endl;
	// 	return 1;
	// }
	// try
	// {
	// 	std::ifstream file(argv[1]);

	// 	std::vector<TOKEN> tokens = Tokenizer::tokenGenerator(file);//

	// 	// printTokens(tokens);

	// 	Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);
	// 	// WebServer->print();

	// 	 WebServer->run();

	// }catch (std::exception &e) {
	// 	std::cout << e.what() << std::endl;
	// }

	// // int x;cin >> x;

	return (0);
}

