#pragma once
#include "includes.hpp"
#include "Location.hpp"
class GenerateLocation {

public:

	GenerateLocation();
	~GenerateLocation();
	Location *generateLocation(std::vector<TOKEN>::iterator &tokens);
private:

};
