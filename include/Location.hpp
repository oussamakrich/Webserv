#include "includes.hpp"
#include <vector>

class Location
{
	private:
		std::string URL; // ex: /test
		std::map<Directives, std::vector<std::string> >  directives;
	public:
		Location();
		Location(const Location &copy);
		Location &operator=(const Location &copy);
		~Location();
};
