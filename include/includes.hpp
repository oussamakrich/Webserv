#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <exception>
#include <fstream>
#include <vector>

typedef enum {
	RETURN,
	LOCATION,
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
	PROXY_PASS,
	TRY_FILES,
	REWRITE,
	AUTO_INDEX,
}Types;
