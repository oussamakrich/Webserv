#include "../include/includes.hpp"
#include "../include/ParseConfig.hpp"

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
		ParseConfig parser(path);


	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
