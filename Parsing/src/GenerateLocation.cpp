/*********************************************************************************
*                              Author: Author Name                               *
*  File Name: /Users/ebennamr/Desktop/WebServer/src/Parse/GenerateLocation.cpp   *
*                    Creation Date: October 19, 2023 05:43 PM                    *
*                    Last Updated: October 22, 2023 12:50 PM                     *
*                              Source Language: cpp                              *
*********************************************************************************/

#include "../include/GenerateLocation.hpp"
#include "../../HttpElement/include/Location.hpp"



Location *GenerateLocation::generateLocation(std::vector<TOKEN>::iterator &tokens)
{
	Location *lc = NULL;

	if(tokens->first == WORD && (tokens + 1)->first == OPEN_C_BRACKET)
	 	lc = new (nothrow) Location(tokens->second);
	else  SyntaxError(ERR_LOCATION);
	if (lc == NULL)  SyntaxError(ERR_OUT_OF_MEM);
	tokens += 2; // skip path and open_c_bracket
	if (tokens->first == RETURN)
		{
			ParsReturn(*lc, ++tokens);
			return lc;
		}
	while (tokens->first != CLOSE_C_BRACKET)
	{
			int tok = tokens->first;
			tokens++;
		switch (tok)
		{
			case ALLOWED_METHOD : ParsAllowedMethod(*lc, tokens);														break;
			case INDEX 			: ParsMultiValue(*lc, tokens, &Location::AddIndex);										break;
			case ERROR_PAGES 	: ParsErrorPage(*lc, tokens); 															break;
			case ROOT			: ParsSingleValue( *lc, tokens, &Location::setRoot , &Location::getRoot); 				break;
			case DEFAULT_TYPE   : ParsSingleValue(*lc, tokens, &Location::setDefaultType, &Location::getDefaultTypes); 	break;
			case AUTO_INDEX 	: ParsAutoIndex(*lc, tokens);  															break;
			case CGI 			: ParsCGI(*lc, tokens);																	break;
			case UPLOAD_PATH 	: ParsSingleValue(*lc, tokens, &Location::setUploadPath, &Location::getUploadPath);		break;
			case UPLOAD			: ParsUpload(*lc, tokens);																break;
			case DOWNLOAD		: ParsDownload (*lc, tokens);															break;
			case RETURN			: SyntaxError(ERR_RETURN_POS);															break;
			default				: SyntaxError("Syntax : Location Invalid  Directive");
		}// end switch
		if (tokens->first != SEMICOLON)   throw std::runtime_error(ERR_MESS_SEMICOLON);
		tokens++;
	}// end while
		lc->final();
		return lc;
} // end GenerateLocation

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
			else throw std::runtime_error(ERR_HTTP_INV_RANGE);
			tokens++;
			if(tokens->first == WORD)
				lc.setRedirectionText(tokens->second);
			else
				 throw std::runtime_error(ERR_SYN_RETURN);
		}
		else if (tokens->first == WORD)
				lc.setRedirectionText(tokens->second);
		else
				 throw std::runtime_error(ERR_SYN_RETURN);
		tokens++;
		if (tokens->first != SEMICOLON || (tokens + 1)->first != CLOSE_C_BRACKET)
			 throw std::runtime_error(tokens->first != SEMICOLON ? ERR_MESS_SEMICOLON : "in return directive");
		tokens++;
		lc.setRedirection(true);

	}
	catch(const std::exception& e)
	{
		SyntaxError(e.what());
	}

}

void GenerateLocation::ParsAllowedMethod(Location &lc, tk_iterator &tokens)
{
		while (tokens->first == WORD)
		{
			if (IS_METHOD_SUPORTED(tokens->second) != true)
				SyntaxError(ERR_MET_NOT_SUPR);
			if (lc.AddHttpMethod(tokens->second) == false) SyntaxError(ERR_MET_REPEAT);
			tokens++;
		}
}

void GenerateLocation::ParsErrorPage(Location &lc, tk_iterator &tokens)
{
		int code = 0; // http code
	try
	{
		if (tokens->first == WORD && isInteger(tokens->second))
		{
			code = std::atoi(tokens->second.c_str());
			if(isInRange(400, 599, code) != true)
				throw std::runtime_error(ERR_HTTP_INV_RANGE);
			tokens++;
			if(tokens->first != WORD)
				throw std::runtime_error(ERR_ERR_PAGE);
		}
		else throw std::runtime_error(ERR_ERR_PAGE);

		if (lc.AddErrorPage(tokens->second, code) == false) throw std::runtime_error(ERR_ERR_PAGE_MULT);
		tokens++;
	}
	catch(const std::exception& e)
	{
		SyntaxError(e.what());
	}

}

void GenerateLocation::ParsMultiValue(Location &lc, tk_iterator &tokens, bool (Location::*fun)(const string &))
{

	int number = 0;
	while (tokens->first == WORD)
	{
		if ((lc.*fun)(tokens->second) == false) SyntaxError(ERR_REPEAT_VAL);
		tokens++;
		number++;
	}
	if(number < 1) SyntaxError(ERR_DIRECTIVE);
}


void GenerateLocation::ParsCGI(Location &lc, tk_iterator &tokens)
{

	int number = 0;
	string key = "";
	if(tokens->first == WORD && (tokens + 1)->first == WORD)
		key  = tokens->second;
		else
			SyntaxError(ERR_CGI_DIRECTIVE);
	tokens++;
	while (tokens->first == WORD)
	{
		if (tokens->second.at(0) != '.')
		SyntaxError(ERR_CGI_EXTENSION);
		if (lc.AddCGI(key, tokens->second) == false) SyntaxError(ERR_REPEAT_VAL);
		tokens++;
		number++;
	}
	if(number < 1) SyntaxError(ERR_CGI_DIRECTIVE);
}



void GenerateLocation::ParsSingleValue(Location &lc, tk_iterator &tokens, void (Location::*set)(const string &), const string &(Location::*get)(void) const)
{
	if (tokens->first == WORD &&  (lc.*get)().size() == 0)
		(lc.*set)(tokens->second);
	else
		SyntaxError(tokens->first == WORD ? ERR_DIRECTIVE : ERR_REPEAT_VAL);
	tokens++;
}

 void GenerateLocation::ParsAutoIndex(Location &lc, tk_iterator &tokens)
{
	if (tokens->first == WORD && (tokens->second == "on" || tokens->second == "off"))
		lc.setAutoIndex(tokens->second == "on");
	else SyntaxError(ERR_AUTOINDEX_VAL);
	tokens++;
}

void		GenerateLocation::ParsUpload(Location &lc, tk_iterator &tokens)
{
	if (tokens->first == WORD && (tokens->second == "on" || tokens->second == "off"))
		lc.setUploadOn(tokens->second == "on");
	else SyntaxError(ERR_UPLOAD_ON_VAL);
	tokens++;

}

void		GenerateLocation::ParsDownload(Location &lc, tk_iterator &tokens)
{
	if (tokens->first == WORD && (tokens->second == "on" || tokens->second == "off"))
		lc.setDownload(tokens->second == "on");
	else SyntaxError(ERR_DOWNLOAD_ON_VAL);
	tokens++;

}

// util
bool isInteger(string &str) { return str.size() && str.find_first_not_of("0123456789") == string::npos; }
bool isInRange(int start, int end, int value)
{
	if (end < start) throw std::out_of_range(ERR_INVALID_RANGE);
	return value >= start &&  value <= end;
}

void SyntaxError(string msg , int code)
{
		std::cerr <<RED <<  "Syntax  Error : "<< RESET << msg << std::endl;
		exit(code);
 }



