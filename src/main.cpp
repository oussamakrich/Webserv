

#include "../include/ParseConfig.hpp"


	
int main (int argc, char **argv) {

	if (argc != 2){
		std::cerr << "No enought arguments" <<	std::endl;
		return 1;
	}
	
	ParseConfig testParse(argv[1]);	



	
	return 0;
}
