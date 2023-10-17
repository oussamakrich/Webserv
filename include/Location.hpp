#include "includes.hpp"
#include <string>
#include <vector>

class Location
{
	private:
		// std::string URL; // ex: /test
		std::map<Directives, std::vector<std::string> >  directives;
	public:
		Location();
		Location(const Location &copy);
		Location &operator=(const Location &copy);
		~Location();
		void fillDirectives(Directives DIR, std::vector<std::string> vect);
		void print();
};
