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
		// req.setBodyBuff("hi");
		req.setBodySize(2);
		req.insertHeader("host", "test");
		req.insertHeader("Port", "test");
		req.insertHeader("Content-type", "");
		req.setQuery("name=dataddffdsfdsf");
		req.setContentLength(0);
		req.setTransferEncoding("shunked");
		req.insertHeader("Protoco", "test");
		string bin = "/bin/bash";
		string path = "hi.sh";
		t_cgiInfo info  = Cgi::Run(req, bin , path);

		int status;
		while (Cgi::isFinished(info, status) == false )
		{
			std::cout << "wait child for finish...\n";
			sleep(1);
		}
		std::cout << "status " << status;
		// Request req(0);
		// req.setBodyBuff(0);
		// req.setBodySize(0);
		// req.insertHeader("host", "test");
		// req.insertHeader("Port", "test");
		// req.insertHeader("Content-type", "test");
		// req.insertHeader("Protoco", "test");
		// string bin = "/bin/bash";
		// string path = "./hi.sh";
		// Cgi::Run(req, bin , path);

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

	// // 	// printTokens(tokens);

	// 	Global *WebServer = GenerateGlobalClass::generateGlobalClass(tokens);
	// 	// WebServer->print();

	// 	 WebServer->run();

	// }catch (std::exception &e) {
	// 	std::cout << e.what() << std::endl;
	// }

	// // int x;cin >> x;

	return (0);
}

