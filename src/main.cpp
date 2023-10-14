
<<<<<<< HEAD
int main (int argc, char **argv) {
=======

#include "../include/ParseConfig.hpp"


	
int main (int argc, char **argv) {

	if (argc != 2){
		std::cerr << "No enought arguments" <<	std::endl;
		return 1;
	}
	
	ParseConfig testParse(argv[1]);	

>>>>>>> bb7b01ba9742e9af15822c9f3a830b12bbb02d59


}
