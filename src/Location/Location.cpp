
#include "../../include/Location.hpp"
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

Location::Location(){};
Location::Location(const Location &copy){*this = copy;};

Location &Location::operator=(const Location &copy){
	this->directives = copy.directives;
	return *this;
};


Location::~Location(){};



void Location::fillDirectives(Directives DIR, std::vector<std::string> vect){
	if (DIR == ALIAS_DIR || DIR == ROOT_DIR || DIR == AUTO_INDEX){
		if (vect.size() != 1)
			throw std::runtime_error("Error: expect one value...");
	}
	std::pair<Directives, std::vector<std::string> >pair(DIR, vect);



	if ((this->directives.insert(pair).second == false)){
		throw std::runtime_error("Error: key repeated twice in Location");
	}

};

void Location::print(){

	std::map<Directives, std::vector<std::string> >::iterator it;
	std::cout << "--------------------- LOCATION---------------------" << std::endl;
	for(it = directives.begin(); it != directives.end(); it++){
		if (it->first == INDEX_DIR)
			std::cout << "index : ";
		if (it->first == AUTO_INDEX)
			std::cout << "auto_index : ";
		else if (it->first == ROOT_DIR)
			std::cout << "ROOT_DIR : ";
		else if (it->first == ALIAS_DIR)
			std::cout << "ALIAS_DIR : ";
		else if (it->first == ALLOWED_METHODS)
			std::cout << "ALLOWED_METHODS : ";

		std::vector<std::string> vect = it->second;
		for (unsigned int i=0; i < vect.size(); i++)
			std::cout << vect[i] << '\t';

		std::cout << std::endl;
	}
	std::cout << "\n\n--------------------- LOCATION---------------------" << std::endl;
}
