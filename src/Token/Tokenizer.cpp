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

std::vector<std::string> Tokenizer::allowedMethods = std::vector<std::string>();
std::map<std::string, Token> Tokenizer::SpecialWords = std::map<std::string, Token>();
std::map<std::string, Token> Tokenizer::SpecialSymbols = std::map<std::string, Token>();
std::stack<std::pair<Token, int> > curlyBrackets = std::stack<std::pair<Token, int> >();
std::stack<std::pair<Token, int> > squareBrackets  = std::stack<std::pair<Token, int> >();

bool Tokenizer::serverFound = false;

Tokenizer::Tokenizer() {
}

Tokenizer::~Tokenizer() {
}



std::string ltrim(const std::string &s) {
    std::string result = s;
    size_t pos = result.find_first_not_of(" \t\n\r");
    if (pos != std::string::npos) {
        result = result.substr(pos);
    }
    return result;
}

std::string rtrim(const std::string &s) {
    std::string result = s;
    size_t pos = result.find_last_not_of(" \t\n\r");
    if (pos != std::string::npos) {
        result = result.substr(0, pos + 1);
    }
    return result;
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}

void Tokenizer::generateTokenMap(void)
{
	allowedMethods.push_back("GET");
	allowedMethods.push_back("POST");
	allowedMethods.push_back("DELETE");
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

// void Tokenizer::tokenizeSpecialSymbols(std::string &line, int lineNumber, int index)
// {

// }

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
		}
		lineNumber++;
	}
	syntaxCheck(tokenizedFile, tokens);
	return tokenizedFile;
}



void Tokenizer::QuotesHandler(std::string &line, int lineNumber, unsigned int  &i, std::vector<t_tokens> &tokens, char quote)
{
	if (line.find(quote, i + 1) == std::string::npos)
	{

		if (quote == '\"')
			std::cout << "Unclosed Double Quotes: line: " << lineNumber << ":" << i;
		else if (quote == '\'')
			std::cout << "Unclosed Single Quotes: line: " << lineNumber << ":" << i;
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

void Tokenizer::SemiColonSyntax(std::vector<t_tokens>::iterator &i, std::vector<t_tokens> &tokens)
{
	if ((i + 1) != tokens.end() && (i + 1)->type != SPACE)
		Tokenizer::fatalError(DIRECTIVE_SYNTAX_ERROR, i);
	if (i != tokens.begin())
	{
		std::vector<t_tokens>::iterator it = i;
		it--;
		while (it != tokens.begin() && it->type == SPACE)
			it--;
		if (it != tokens.begin() && it->type != SEMICOLON && it->type != OPEN_C_BRACKET && it->type != CLOSE_C_BRACKET)
			Tokenizer::fatalError(MISSING_SEMICOLON, it);
	}
}

void Tokenizer::BlockHandler(std::vector<TOKEN> &tokenizedFile, std::vector<t_tokens> &tokens, std::vector<t_tokens>::iterator &i)
{
	std::vector<t_tokens>::iterator type = i;
	i++;
	while (i != tokens.end() && i->type == SPACE)
		i++;
	if (i == tokens.end() || i->type != WORD)
		Tokenizer::fatalError(MISSING_URL_BLOCK, type);
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
	i++;
	while (i != tokens.end() && i->type == SPACE)
		i++;
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
		}
		else if (i->type != SPACE){
			if (i->type == QUOTES)
				i->type = WORD;
			tokenizedFile.push_back(std::make_pair(i->type, i->value));
		}
		i++;
	}
	if (i == tokens.end())
		Tokenizer::fatalError(UNCLOSED_BRACKETS, type);
	tokenizedFile.push_back(std::make_pair(i->type, i->value));

}

bool Tokenizer::syntaxCheck(std::vector<TOKEN> &tokenizedFile, std::vector<t_tokens> &tokens)
{
	std::vector<t_tokens>::iterator i;

	serverFound = false;
	for (i = tokens.begin(); i != tokens.end(); i++)
	{
		if (i->type == WORD && SpecialWords.find(i->value) != SpecialWords.end())
		{
			tokenizedFile.push_back(std::make_pair(SpecialWords[i->value], i->value));
			i->type = tokenizedFile.back().first;
			if (i->type != TYPES && i->type != SERVER && i->type != LOCATION)
				Tokenizer::SemiColonSyntax(i, tokens);
			else if (i->type == TYPES || i->type == LOCATION)
				Tokenizer::BlockHandler(tokenizedFile, tokens, i);
		}
		else if (i->type != SPACE)
		{
			if (i->type == QUOTES)
				i->type = WORD;
			if (i->type == WORD)
			{
				std::vector<t_tokens>::iterator it = i;
				it--;
				while (it != tokens.begin() && it->type == SPACE)
					it--;
				if (it != tokens.begin() && (it->type == SEMICOLON || it->type == CLOSE_C_BRACKET || it->type == OPEN_C_BRACKET))
					Tokenizer::fatalError(UNKNOWN_DIRECTIVE, i);
			}
			tokenizedFile.push_back(std::make_pair(i->type, i->value));
		}

	}
	return true;
}



bool Tokenizer::is_not_of(char c, std::string str)
{
	if (str.find(c) == std::string::npos)
		return true;
	return false;
}

void Tokenizer::fatalError(Error error,std::vector<t_tokens>::iterator &i)
{
	if (error == DIRECTIVE_SYNTAX_ERROR)
	{
		std::cout << RED"Syntax Error:" << RESET" line " << i->column << ":" << (i+1)->row << " expected space after " << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (DIRECTIVE_SYNTAX_ERROR)\n" << RESET;
	}
	else if (error == UNKNOWN_DIRECTIVE)
	{
		std::cout << RED"Syntax Error:" << RESET" line " << i->column << ":" << (i+1)->row << " unknown directive " << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (UNKNOWN_DIRECTIVE)\n" << RESET;
	}
	else if (error == MISSING_SEMICOLON)
	{
		std::cout << RED"Syntax Error:" << RESET" line " << i->column << ":" << (i)->row << " missing semicolon after " << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (MISSING_SEMICOLON)\n" << RESET;
	}
	else if (error == UNCLOSED_BRACKETS)
	{
		std::cout << RED"Syntax Error:" << RESET" line " << i->column << ":" << (i)->row << " unclosed or missing open bracket." << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (UNCLOSED_BRACKETS)\n" << RESET;
	}
	else if (error == NESTED_BLOCKS_NOT_ALLOWED)
	{
		std::cout << RED"Syntax Error:" << RESET" line " << i->column << ":" << (i)->row << " nested blocks are not allowed." << i->value << ".\n\n" ;
		std::cout << RED"\t\tError code: (NESTED_BLOCKS_NOT_ALLOWED)\n" << RESET;
	}
	else if (error == MISSING_URL_BLOCK)
	{
		std::cout << RED"Syntax Error:" << RESET" line " << i->column << ":" << (i)->row << " missing url '" << i->value << "'.\n\n" ;
		std::cout << RED"\t\tError code: (MISSING_URL_BLOCK)\n" << RESET;
	}
	exit(1);
}
