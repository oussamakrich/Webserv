#pragma once
#include "../../include/includes.hpp"
#include "../../HttpElement/include/Location.hpp"

#define ERR_MET_NOT_SUPR 	"Method not suported"
#define ERR_INVALID_RANGE 	"Exception : Invalid Range ! end must be big than start\n"
#define ERR_MET_REPEAT 		"Method Repeated !"
#define ERR_LOCATION 		"Location messing path or open curly bracket }"
#define ERR_HTTP_INV_RANGE 	"invalid http code number"
#define ERR_MESS_SEMICOLON	"messing semicolon [ ; ]"
#define ERR_OUT_OF_MEM      "Fatal Error : out of memory"
#define ERR_SYN_RETURN 		"return directive"
#define ERR_ERR_PAGE        "error_page  directive"
#define ERR_ERR_PAGE_MULT   "error_page  \"multi page of code\""
#define ERR_REPEAT_VAL      "Repeat value"
#define ERR_AUTOINDEX_VAL 	"AutoIndex accept on/off value"
#define ERR_UPLOAD_ON_VAL 	"Upload  accept on/off value"
#define ERR_DOWNLOAD_ON_VAL "Dowload accept on/off value"
#define ERR_DIRECTIVE    	"Directive must be followed by value(s)"
#define ERR_CGI_DIRECTIVE 	"Loaction::cgi , \n hint : cgi php-cgi .php\n"
#define ERR_CGI_EXTENSION 	"CGI extension must be start with '.'\n example .php .py ... "
#define ERR_RETURN_POS		"Return : use in location  :\n\nloction /path {\n		return code text;\n}"
#define ERR_UPLOAD_PATH 	"Location -> Upload  Path Require "
#define ERR_ROOT_REQUIRE  	"Location -> Root  Path Require"

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
		static void		ParsUpload(Location &lc, tk_iterator &tokens);
		static void		ParsDownload(Location &lc, tk_iterator &tokens);
		static void 	ParsCGI(Location &lc, tk_iterator &tokens);

};
//***************************** util ***************************************
		bool isInteger(string &str); // [0-9] one more digit
		bool isInRange(int start, int end, int value); // if  end < start  throw exception value E[start , end]
		void SyntaxError(string msg="Fatal Error", int code=1);

