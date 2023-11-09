 
#pragma once
#include "../../HttpElement/include/Request.hpp"
#include "../../Parsing/include/GenerateLocation.hpp" // use  for Isinger() function
#include "../../Request/Buffer/RequestBuffer.hpp"


class ParsRequest
{
public :
		static Request *Pars(RequestBuffer &reqBuff);
		static void ParsFirstLine(Request& req, RequestBuffer &reqBuff) ;
		static bool isValidKey(std::string key);
		static bool ParsHeaders(Request& req, std::string& line);
};

//***************************** util ***************************************
std::string replaceAll(std::string str, const std::string s1, const std::string s2);
std::vector<std::string> split(const std::string& str, char delimiter);
int getMethodCode(std::string  method);

