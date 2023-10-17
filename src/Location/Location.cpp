#include "../../include/Location.hpp"

// Canonical Form:

Location::~Location() {}

Location::Location() {}

Location::Location(const Location &copy) {
	*this = copy;
}

Location &Location::operator=(const Location &copy) {
	if (this != &copy) {
		this->URL = copy.URL;
		this->directives = copy.directives;
	}
	return *this;
}

// End Canonical Form.
