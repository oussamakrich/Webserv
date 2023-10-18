#include "../include/includes.hpp"
#include "../include/ParseConfig.hpp"
#include "../include/Request.hpp"
#include <fstream>
#include <sstream>
int main (int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: ./webserv <path_to_config_file>" << std::endl;
		return (1);
	}
	try
	{
		std::string path(argv[1]);
		// ParseConfig parser(path);
		std::ifstream ss("request_example");
		if (!ss.is_open())
			throw std::runtime_error("Error: Request file not found.");

		std::string request;
		std::stringstream buffer;
		buffer << ss.rdbuf();

		request = buffer.str();

		// Request req(request);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
