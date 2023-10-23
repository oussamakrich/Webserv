#include "../../include/Tokenizer.hpp"
#include <_types/_intmax_t.h>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::map<std::string, Token> Tokenizer::SpecialWords = std::map<std::string, Token>();
std::map<std::string, Token> Tokenizer::SpecialSymbols = std::map<std::string, Token>();
std::stack<Token> Tokenizer::curlyBrackets = std::stack<Token>();
int Tokenizer::serverFound = 0;

Tokenizer::Tokenizer() {}

Tokenizer::~Tokenizer() {}

void Tokenizer::generateTokenMap(void){
SpecialWords["return"] = RETURN;
SpecialWords["location"] = LOCATION;
SpecialWords["server"] = SERVER;
SpecialWords["include"] = INCLUDE;
SpecialWords["types"] = TYPES;
SpecialWords["listen"] = LISTEN;
SpecialWords["root"] = ROOT;
SpecialWords["server_name"] = SERVER_NAME;
SpecialWords["index"] = INDEX;
SpecialWords["max_body_size"] = MAX_BODY_SIZE;
SpecialWords["error_page"] = ERROR_PAGES;
SpecialWords["default_type"] = DEFAULT_TYPE;
SpecialWords["error_log"] = ERROR_LOG;
SpecialWords["access_log"] = ACCESS_LOG;
SpecialWords["try_files"] = TRY_FILES;
SpecialWords["rewrite"] = REWRITE;
SpecialWords["autoindex"] = AUTO_INDEX;
SpecialWords["allowed_methods"] = ALLOWED_METHOD;
SpecialSymbols["["] = OPEN_S_BRACKET;
SpecialSymbols["]"] = CLOSE_S_BRACKET;
SpecialSymbols["{"] = OPEN_C_BRACKET;
SpecialSymbols["}"] = CLOSE_C_BRACKET;
SpecialSymbols[";"] = SEMICOLON;
}

void Tokenizer::WordHandler(std::string &line, int lineNumber, unsigned int &i, std::vector<t_tokens> &tokens)
{
	unsigned int len = i;
	while (len < line.length() && Tokenizer::is_not_of(line[len], " \t\r;{}"))
		len++;
	tokens.push_back((t_tokens){
		WORD,
		line.substr(i, (len - i)),
		lineNumber,
		i
	});
	i += (len - i) - 1;
}

void Tokenizer::QuotesHandler(std::string &line, int lineNumber, unsigned int  &i, std::vector<t_tokens> &tokens, char quote)
{
	if (line.find(quote, i + 1) == std::string::npos)
	{
		if (quote == '\"')
			std::cout << "Unclosed Double Quotes: line: " << U_WHITE"config.conf:" << lineNumber << ":" << i;
		else if (quote == '\'')
			std::cout << "Unclosed Single Quotes: line: " << U_WHITE"config.conf:" << lineNumber << ":" << i;
		exit(1);
	}
	tokens.push_back((t_tokens){
		QUOTES,
		line.substr(i+1, ((line.find(quote, i + 1) - i) - 1)),
		lineNumber,
		i,
		 });
	i += (tokens.back().value.length() + 1);
}

std::vector<TOKEN> Tokenizer::tokenGenerator(std::ifstream &file)
{
	std::vector<t_tokens>	tokens;
	int					lineNumber = 1;
	std::string			line;
	std::vector<TOKEN> 	tokenizedFile;
	Tokenizer::generateTokenMap();
	while (std::getline(file, line))
	{
		for (unsigned int i = 0; i < line.length(); i++)
		{
			if (std::isspace(line[i]))
				tokens.push_back((t_tokens){SPACE, " ", lineNumber, i});
			else if (line[i] == '{')
				tokens.push_back((t_tokens){OPEN_C_BRACKET, "{", lineNumber, i});
			else if (line[i] == '}')
				tokens.push_back((t_tokens){CLOSE_C_BRACKET, "}", lineNumber, i});
			else if (line[i] == ';')
				tokens.push_back((t_tokens){SEMICOLON, ";", lineNumber, i});
			else if (line[i] == '\"')
				Tokenizer::QuotesHandler(line, lineNumber, i, tokens, line[i]);
			else if (line[i] == '\'')
				Tokenizer::QuotesHandler(line, lineNumber, i, tokens, line[i]);
			else
				Tokenizer::WordHandler(line, lineNumber, i, tokens);
		}
		lineNumber++;
	}
	syntaxCheck(tokenizedFile, tokens);
	return tokenizedFile;
}

void Tokenizer::SemiColonSyntax(std::vector<t_tokens>::iterator &i, std::vector<t_tokens> &tokens)
{
	if (i == tokens.begin())
		Tokenizer::fatalError(DIRECTIVE_SYNTAX_ERROR, i);
	std::vector<t_tokens>::iterator it = i;
	it--;
	while (it != tokens.begin() && it->type == SPACE)
		it--;
	if (it != tokens.begin() && it->type != SEMICOLON && it->type != OPEN_C_BRACKET && it->type != CLOSE_C_BRACKET)
		Tokenizer::fatalError(MISSING_SEMICOLON, it);
}

void Tokenizer::BlockHandler(std::vector<TOKEN> &tokenizedFile, std::vector<t_tokens> &tokens, std::vector<t_tokens>::iterator &i)
{
	bool isLocation = (i->type == LOCATION);

	std::vector<t_tokens>::iterator type = i;
	i++;
	while (i != tokens.end() && i->type == SPACE && isLocation) i++;
	if ((i == tokens.end() || i->type != WORD) && isLocation)
		Tokenizer::fatalError(MISSING_URL_BLOCK, type);
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
	if (isLocation) i++;
	while (i != tokens.end() && i->type == SPACE) i++;
	if (i == tokens.end() || i->type != OPEN_C_BRACKET)
		Tokenizer::fatalError(UNCLOSED_BRACKETS, type);
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
	i++;
	while (i != tokens.end() && i->type != CLOSE_C_BRACKET)
	{
		if (i->type == OPEN_C_BRACKET)
			Tokenizer::fatalError(NESTED_BLOCKS_NOT_ALLOWED, type);
		else if (i->type == WORD && SpecialWords.find(i->value) != SpecialWords.end())
		{
			tokenizedFile.push_back(std::make_pair(SpecialWords.find(i->value)->second, i->value));
			Tokenizer::SemiColonSyntax(i, tokens); // REMOVE this if you are already checking for semicolon in the parsing part.
		}
		else if (i->type != SPACE)
		{
			if (i->type == QUOTES) i->type = WORD;
			tokenizedFile.push_back(std::make_pair(i->type, i->value));
			if (i->type == WORD && !isLocation && i->value.find('/') != std::string::npos)
				Tokenizer::SemiColonSyntax(i, tokens); // REMOVE this if you are already checking for semicolon in the parsing part.
		}
		i++;
	}
	if (i == tokens.end())
		Tokenizer::fatalError(UNCLOSED_BRACKETS, type);
	Tokenizer::SemiColonSyntax(i, tokens); // REMOVE this if you are already checking for semicolon in the parsing part.
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
}

void Tokenizer::ServerHandler(TOKENIZED_FILE &tokenizedFile, TOKEN_STRUCTS &tokens, std::vector<t_tokens>::iterator &i)
{

	Tokenizer::serverFound++;
	i++;
	while (i != tokens.end() && i->type == SPACE)
		i++;
	if (i == tokens.end() || i->type != OPEN_C_BRACKET)
		Tokenizer::fatalError(UNCLOSED_BRACKETS, --i);
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
	Tokenizer::curlyBrackets.push(OPEN_C_BRACKET);
}


void Tokenizer::WordSyntax(std::vector<t_tokens>::iterator &i, std::vector<t_tokens> &tokens)
{
	if (i->type == QUOTES) i->type = WORD;
	if (i->type == WORD)
	{
		std::vector<t_tokens>::iterator it = i;
		if (i == tokens.begin())
			Tokenizer::fatalError(UNKNOWN_DIRECTIVE, i);
		it--;
		while (it != tokens.begin() && it->type == SPACE)
			it--;
		if (it != tokens.begin() && (it->type == SEMICOLON || it->type == CLOSE_C_BRACKET || it->type == OPEN_C_BRACKET))
			Tokenizer::fatalError(UNKNOWN_DIRECTIVE, i);
	}
}

void Tokenizer::CloseCurlyBracketsSyntax(std::vector<t_tokens>::iterator &i, std::vector<t_tokens> &tokens, std::vector<TOKEN> &tokenizedFile)
{
	Tokenizer::SemiColonSyntax(i, tokens);
	if (!Tokenizer::curlyBrackets.empty())
		Tokenizer::curlyBrackets.pop();
	else
		Tokenizer::fatalError(UNCLOSED_BRACKETS, i);
	std::vector<t_tokens>::iterator it = i;
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
	it++;
	while (it != tokens.end() && it->type == SPACE)
		it++;
	if (it != tokens.end() && SpecialWords[it->value] != SERVER)
		Tokenizer::fatalError(UNCLOSED_BRACKETS, i);
}


bool Tokenizer::syntaxCheck(std::vector<TOKEN> &tokenizedFile, std::vector<t_tokens> &tokens)
{
	std::vector<t_tokens>::iterator i;

	for (i = tokens.begin(); i != tokens.end(); i++)
	{
		if (i->type == WORD && SpecialWords.find(i->value) != SpecialWords.end())
		{
			tokenizedFile.push_back(std::make_pair(SpecialWords[i->value], i->value));
			i->type = tokenizedFile.back().first;
			if (i->type != SERVER)
				Tokenizer::SemiColonSyntax(i, tokens);
			if (i->type == TYPES || i->type == LOCATION)
				Tokenizer::BlockHandler(tokenizedFile, tokens, i);
			else if (i->type == SERVER)
			{
				if (curlyBrackets.size() != 0)
					Tokenizer::fatalError(UNCLOSED_BRACKETS, i);
				Tokenizer::ServerHandler(tokenizedFile, tokens, i);
			}
		}
		else if (i->type != SPACE)
		{
			Tokenizer::WordSyntax(i, tokens);
			if (i->type == CLOSE_C_BRACKET)
				Tokenizer::CloseCurlyBracketsSyntax(i, tokens, tokenizedFile);
			else
				tokenizedFile.push_back(std::make_pair(i->type, i->value));
		}
		if (i == tokens.end()) break;
	}
	if (Tokenizer::serverFound == 0)
		Tokenizer::fatalError(SERVER_BLOCK_NOT_FOUND, i);
	if (!Tokenizer::curlyBrackets.empty())
		Tokenizer::fatalError(UNCLOSED_BRACKETS, --i);
	return true;
}

bool Tokenizer::is_not_of(char c, std::string str) {return (str.find(c) == std::string::npos);}

void Tokenizer::fatalError(Error error,std::vector<t_tokens>::iterator &i)
{
	if (error == DIRECTIVE_SYNTAX_ERROR)
	{
		std::cout << RED"Syntax Error:" << RESET" line " <<U_WHITE"config.conf:" << i->column << ":" << (i)->row << RESET" expected space after " << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (DIRECTIVE_SYNTAX_ERROR)\n" << RESET;
	}
	else if (error == UNKNOWN_DIRECTIVE)
	{
		std::cout << RED"Syntax Error:" << RESET" line " <<U_WHITE"config.conf:" << i->column << ":" << (i)->row << RESET " unknown directive " << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (UNKNOWN_DIRECTIVE)\n" << RESET;
	}
	else if (error == MISSING_SEMICOLON)
	{
		std::cout << RED"Syntax Error:" << RESET" line " <<U_WHITE"config.conf:" << i->column << ":" << (i)->row << RESET " missing semicolon after " << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (MISSING_SEMICOLON)\n" << RESET;
	}
	else if (error == UNCLOSED_BRACKETS)
	{
		std::cout << RED"Syntax Error:" << RESET" line " <<U_WHITE"config.conf:" << i->column << ":" << (i)->row << RESET " unclosed or missing open bracket." << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (UNCLOSED_BRACKETS)\n" << RESET;
	}
	else if (error == NESTED_BLOCKS_NOT_ALLOWED)
	{
		std::cout << RED"Syntax Error:" << RESET" line " <<U_WHITE"config.conf:" << i->column << ":" << (i)->row << RESET " nested blocks are not allowed." << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (NESTED_BLOCKS_NOT_ALLOWED)\n" << RESET;
	}
	else if (error == MISSING_URL_BLOCK)
	{
		std::cout << RED"Syntax Error:" << RESET" line " <<U_WHITE"config.conf:" << i->column << ":" << (i)->row << RESET " missing url '" << i->value << "'.\n\n" ;
		std::cout << RED"\t\tError code: (MISSING_URL_BLOCK)\n" << RESET;
	}
	else if (error == SERVER_BLOCK_NOT_FOUND)
	{
		std::cout << RED"Syntax Error:" << RESET" line " <<U_WHITE"config.conf:" << 0 << ":" << 0 << RESET " server block not found.\n\n" ;
		std::cout << RED"\t\tError code: (SERVER_BLOCK_NOT_FOUND)\n" << RESET;
	}
	exit(error);
}




