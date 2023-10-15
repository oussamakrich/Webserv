

#include "../include/ParseConfig.hpp"
#include <string>

int main (int argc, char **argv) {

	if (argc != 2){
		std::cerr << "No enought args" << std::endl;
		return 1;
	}
	std::string path = argv[1];
	try {
		ParseConfig cc(path);
	}
	catch (std::exception &e){
			std::cout << e.what() << std::endl;
	}
}
