#pragma once
#include "includes.hpp"
#include <vector>

class Location {
	private:// Add what you need here.
		std::vector<std::string> Allowed_methods;
		std::string index;
		std::string root;
		std::string autoindex;
		std::vector<std::string> error_pages;
		std::vector<std::pair<int, std::string> > redirect;

	public: //Setters:

	public: //Getters:


	public:
		Location();
		~Location();
};
