#pragma once
#include "../../include/includes.hpp"
#include "../../HttpElement/include/Location.hpp"
using namespace std;

class GenerateLocation
{
	typedef std::vector<TOKEN>::iterator  tk_iterator;
public:

	static Location *generateLocation(std::vector<TOKEN>::iterator &tokens);
private:

//***************************** pars  ***************************************

		static	void	ParsReturn(Location &lc, tk_iterator &tokens);
		static void		ParsAllowedMethod(Location &lc, tk_iterator &tokens);
		static void		ParsErrorPage(Location &lc, tk_iterator &tokens);
		static void		ParsMultiValue(Location &lc, tk_iterator &tokens, bool (Location::*f)(const string &)); // index try_files e
		static void		ParsSingleValue(Location &lc, tk_iterator &tokens, void (Location::*set)(const string &), const string& (Location::*get)(void) const);
		static void		ParsAutoIndex(Location &lc, tk_iterator &tokens);

};
//***************************** util ***************************************
		bool isInteger(string &str); // [0-9] one more digit
	 	bool isInRange(int start, int end, int value); // if  end < start  throw exception value E[start , end]
		void SyntaxError(string msg="Fatal Error", int code=1);
