/*********************************************************************************
*                              Author: Author Name                               *
*  File Name: /Users/ebennamr/Desktop/WebServer/src/Parse/GenerateLocation.cpp   *
*                    Creation Date: October 19, 2023 05:43 PM                    *
*                    Last Updated: October 22, 2023 12:50 PM                     *
*                              Source Language: cpp                              *
*********************************************************************************/

#include "../../include/GenerateLocation.hpp"
#include "../../include/Location.hpp"

#define EXP_INVALID_RANGE 	"Exception : Invalid Range ! end must be big than start\n"
#define EXP_MET_NOT_SUPR  	"Syntax Error : Method not Suported"
#define EXP_MET_REPEAT 		"Syntax Error : Method Repeated !"
#define EXP_HTTP_INV_RANGE 	"error  : Redirection code  must be in  range [100, 599]"
#define EXP_MESS_SEMICOLON	"error  : messing semicolon [ ; ]"
#define IS_METHOD_SUPORTED(method)((method == "GET" || method == "DELETE" || method == "POST" ))

Location *GenerateLocation::generateLocation(std::vector<TOKEN>::iterator &tokens)
{
	Location *lc;

	if(tokens->first == WORD && (tokens + 1)->first == OPEN_C_BRACKET) 
	 	lc = new (nothrow) Location(tokens->second);
	else  FatalError("location Syntax -> ");
	if (lc == NULL)  FatalError("fail alloc :  out of memory");
	tokens += 2; // skip path and open_c_bracket
	while (tokens->first != CLOSE_C_BRACKET)
	{	
		if (tokens->first == RETURN)
			{
			}
		switch (tokens->first)
		{
			case ROOT			: ParsSingleValue( *lc, tokens, &Location::setRoot , &Location::getRoot); 				break;
			case ALLOWED_METHOD : ParsAllowedMethod(*lc, tokens);														break; 
			case ERROR_PAGES 	: ParsErrorPage(*lc, tokens); 															break;
			case INDEX 			: ParsMultiValue(*lc, tokens, &Location::AddIndex);										break;
			case DEFAULT_TYPE   : ParsSingleValue(*lc, tokens, &Location::setDefaultType, &Location::getDefaultTypes); 	break;
			case TRY_FILES 		: ParsMultiValue(*lc, tokens, &Location::AddTryFile); 									break;
			case AUTO_INDEX 	: ParsAutoIndex(*lc, tokens);  															break;
			default				: FatalError();
		}// end switch 
		
	}// end while
	
}// end GenerateLocation 

void	GenerateLocation::ParsReturn(Location &lc, tk_iterator &tokens)
{
	int code = 0; // http code 
	try
	{
		if (tokens->first == WORD && isInteger(tokens->second))
		{
			code = std::atoi(tokens->second.c_str());
			if(isInRange(100, 599, code) == true)   
				lc.setRedirectionCode(code);
			else throw std::runtime_error(EXP_HTTP_INV_RANGE);
			tokens++;
			if(tokens->first == WORD) 
				lc.setRedirectionText(tokens->second);
			else
				 throw std::runtime_error("error  :return Syntax error ");
		}
		else if (tokens->first == WORD)
				lc.setRedirectionText(tokens->second);
			else
				 throw std::runtime_error("error  :return Syntax error ");
		tokens++;
		if (tokens->first != SEMICOLON) throw std::runtime_error(EXP_MESS_SEMICOLON);
		tokens++;
	}
	catch(const std::exception& e)
	{
		FatalError(e.what());
	}
	lc.setRedirection(true);
}

void GenerateLocation::ParsAllowedMethod(Location &lc, tk_iterator &tokens)
{
		while (tokens->first == WORD)
		{
			if (IS_METHOD_SUPORTED(tokens->second) != true)
				FatalError(EXP_MET_NOT_SUPR);
			if (lc.AddHttpMethod(tokens->second) == false) FatalError(EXP_MET_REPEAT);
			tokens++;
		}
		if (tokens->first == SEMICOLON) tokens++;
		else  throw std::runtime_error(EXP_MESS_SEMICOLON);
}

void GenerateLocation::ParsErrorPage(Location &lc, tk_iterator &tokens)
{
		int code = 0; // http code 
	try
	{
		if (tokens->first == WORD && isInteger(tokens->second))
		{
			code = std::atoi(tokens->second.c_str());
			if(isInRange(100, 599, code) != true)   
				throw std::runtime_error(EXP_HTTP_INV_RANGE);
			tokens++;
			if(tokens->first != WORD) 
				throw std::runtime_error("error  : error_page ->  Syntax error ");
		}
			else throw std::runtime_error("error  : error_page ->  Syntax error ");

		if (lc.AddErrorPage(tokens->second, code) == false) throw std::runtime_error("error : error_page : multi page of code ");
		tokens++;
		if (tokens->first != SEMICOLON) throw std::runtime_error(EXP_MESS_SEMICOLON);
	}
	catch(const std::exception& e)
	{
		FatalError(e.what());
	}
	
}
void GenerateLocation::ParsMultiValue(Location &lc, tk_iterator &tokens, bool (Location::*fun)(const string &))
{
	while (tokens->first == WORD)
	{
		if ((lc.*fun)(tokens->second) == false) FatalError("Repeat value");
		tokens++;
	}
	if (tokens->first != SEMICOLON) throw std::runtime_error(EXP_MESS_SEMICOLON);
	tokens++;
}

void GenerateLocation::ParsSingleValue(Location &lc, tk_iterator &tokens, void (Location::*set)(const string &), const string &(Location::*get)(void) const)
{
	if (tokens->first == WORD &&  (lc.*get)().size() != 0)
		(lc.*set)(tokens->second);
	else FatalError();
	tokens++;
	if (tokens->first != SEMICOLON) throw std::runtime_error(EXP_MESS_SEMICOLON);
}

 void GenerateLocation::ParsAutoIndex(Location lc, tk_iterator &tokens)
{
	if (tokens->first == WORD && (tokens->second == "on" || tokens->second == "off"))
		lc.setAutoIndex(tokens->second == "on");
	else FatalError("Error : AutoIndex accept on/off value");
	tokens++;
	if (tokens->first != SEMICOLON) throw std::runtime_error(EXP_MESS_SEMICOLON);
}
// util 
bool isInteger(string &str) { return str.size() && str.find_first_not_of("0123456789") == string::npos; }
bool isInRange(int start, int end, int value)
{
	if (end < start) throw std::out_of_range(EXP_INVALID_RANGE);
	return value >= start &&  value <= end;

}

void FatalError(string msg="error : Unknow error", int code=1)
{
		std::cerr << msg << std::endl;
		exit(code);
 }



