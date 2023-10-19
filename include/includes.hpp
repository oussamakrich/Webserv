#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <exception>
#include <fstream>
#include <vector>

#define TOKEN std::pair<Token, std::string>

typedef enum {
	RETURN,
	LOCATION,
	WORD,
	SERVER,
	OPEN_S_BRACKET,
	CLOSE_S_BRACKET,
	OPEN_C_BRACKET,
	CLOSE_C_BRACKET,
	SEMICOLON,
	INCLUDE,
	ALLOWED_METHOD,
	COLON,
	TYPES,
	SPACE,
	HOST,
	PORT,
	ROOT,
	SERVER_NAME,
	INDEX,
	MAX_BODY_SIZE,
	ERROR_PAGES,
	DEFAULT_TYPE,
	ERROR_LOG,
	ACCESS_LOG,
	TRY_FILES,
	REWRITE,
	AUTO_INDEX,
}Token;
