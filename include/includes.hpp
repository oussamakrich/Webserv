#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <exception>
#include <fstream>
#include <vector>


// Start: Macros for error messages.
#define CONFIG_FILE_NOT_FOUND "Error: Config file not found."

#define ERR_OPEN "Error: while opning file"
#define ERR_CONFIGFILE "Error: config file syntax error"
#define UNCLOSED "Error: unclosed Bracket"

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
	LOCATION,
	CURLYOPEN,
	CURLYCLOSE,
	TYPES,
	LIMIT,
	END,
}token;

typedef enum {
	HOST,
	PORT,
	ROOT,
	SERVER_NAME,
	INDEX,
	CLIENT_MAX_BODY_SIZE,
	ERROR_PAGE,
	DEFAULT_TYPE,
	ERROR_LOG,
	ACCESS_LOG,
	ALLOW_METHODS,
}allowedRules;

std::string trim(const std::string &s);

