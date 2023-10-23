#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <exception>
#include <fstream>
#include <vector>


#define TOKEN std::pair<Token, std::string>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"
#define U_WHITE "\033[4;37m"

typedef enum {
	DIRECTIVE_SYNTAX_ERROR,
	UNKNOWN_DIRECTIVE,
	MISSING_SEMICOLON,
	UNCLOSED_BRACKETS,
	NESTED_BLOCKS_NOT_ALLOWED,
	MISSING_URL_BLOCK,
	SERVER_BLOCK_NOT_FOUND,
}Error;


typedef enum {
	RETURN,
	LOCATION,
	SERVER,
	INCLUDE,
	TYPES,
	MIME_TYPE,
	LISTEN,
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
	ALLOWED_METHOD,
	OPEN_S_BRACKET,
	CLOSE_S_BRACKET,
	OPEN_C_BRACKET,
	CLOSE_C_BRACKET,
	SEMICOLON,
	COLON,
	SPACE,
	WORD,
	QUOTES
}Token;

typedef struct {
	Token		type;
	std::string	value;
	int			column;
	unsigned int			row;
}t_tokens;

std::string trim(const std::string &s);



// Printers: this section will be removed its for debugging purposes only.
void printTokens(std::vector<TOKEN> tokens);
void printEnam(Token t);
