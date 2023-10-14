#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <exception>
#include <fstream>
#include <vector>

#define ERR_OPEN "Error: while opning file"
#define ERR_CONFIGFILE "Error: config file syntax error"


#define TOKEN_PAIR std::pair<std::string, token> 

 typedef enum{
	INCLUDE,
	SEMICOLONE,
	COLONE,
	SERVER,
	BRACKETOPEN,
	BRACKETCLOSE,
	ESP,
	VALUE,
}token;
