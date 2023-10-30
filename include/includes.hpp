#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <exception>
#include <fstream>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
# include <sys/types.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <unistd.h>
#include <sys/poll.h>
#include <string>


#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"
#define U_WHITE "\033[4;37m"
#define YELLOW "\033[0;33m"


#define U_YELLOW "\033[4;33m"
#define BLUE "\033[0;34m"

#define IS_METHOD_SUPORTED(method)((method == "GET" || method == "DELETE" || method == "POST" ))

typedef enum {
	DIRECTIVE_SYNTAX_ERROR,
	UNKNOWN_DIRECTIVE,
	MISSING_SEMICOLON,
	UNCLOSED_BRACKETS,
	NESTED_BLOCKS_NOT_ALLOWED,
	MISSING_URL_BLOCK,
	SERVER_BLOCK_NOT_FOUND,
	EMPTY_QUOTES,
	EMPTY_BLOCK,
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

#define TOKEN std::pair<Token, std::string>
#define TOKEN_PAIR std::pair<Token, std::string>
#define TOKEN_IT std::vector<TOKEN_PAIR>::iterator
#define TOKEN_VECT std::vector<TOKEN_PAIR>

#define ERRPAGE_PAIR std::pair<int, std::string>


#define CONFIG_GUID "Configuration file is written in the following format:\ninclude path/to/mime.types;\nserver [\n 	location {\n 		...\n 	};\n]\n\nserver [\n 	...\n]\n\nserver [\n 	...\n]\n\n\n--------------------------------------\n* include: is used to include a mime.types file its optional.\n* server: is used to define a server block.\n* location: is used to define a location block.\n* types: is used to define a types block.\n* port: is used to define a port to listen on.\n* root: is used to define a root directory.\n* server_name: is used to define a server name.\n* index: is used to define a index file.\n* max_body_size: is used to define a max body size.\n* error_pages: is used to define a error pages in this formal: error_page 404 [error code] /*.html;\n	all error codes will be redirected to the specified html file and the '*' will be replaced by a number.\n* default_type: is used to define a default type."


std::string trim(const std::string &s);

std::string UrlDecode(std::string codedStr);
std::string UrlEncode(std::string simpleStr);

void Otrim(std::string &line);
void error(std::string error);

// Printers: this section will be removed its for debugging purposes only.
void printTokens(std::vector<TOKEN> tokens);
void printEnam(Token t);
