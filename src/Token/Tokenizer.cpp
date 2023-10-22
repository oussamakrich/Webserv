#include "../../include/Tokenizer.hpp"
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
	int					lineNumber = 0;
	std::string			line;
	std::vector<TOKEN> 	tokenizedFile;
	Tokenizer::generateTokenMap();
	while (std::getline(file, line))
	{
		for (unsigned int i = 0; i < line.length(); i++)
		{
			if (std::isspace(line[i]))
				tokens.push_back((t_tokens){SPACE, " ", lineNumber, i});
			// else if (line[i] == '[')
			// 	tokens.push_back((t_tokens){OPEN_S_BRACKET, "[", lineNumber, i});
			// else if (line[i] == ']')
			// 	tokens.push_back((t_tokens){CLOSE_S_BRACKET, "]", lineNumber, i});
			else if (line[i] == '{')
				tokens.push_back((t_tokens){OPEN_C_BRACKET, "{", lineNumber, i});
			else if (line[i] == '}')
				tokens.push_back((t_tokens){CLOSE_C_BRACKET, "}", lineNumber, i});
			else if (line[i] == ';')
				tokens.push_back((t_tokens){SEMICOLON, ";", lineNumber, i});
			else if (line[i] == '\"')
			{
				if (line.find('\"', i+1) == std::string::npos)
				{
					std::cout << "Unclosed double Quotes:" << lineNumber << ":" << i;
					exit(1);
				}
				tokens.push_back((t_tokens){
					WORD,
					line.substr(i+1, ((line.find('\"', i + 1) - i) - 1)),
					lineNumber,
					i,
					 });
				i += tokens.back().value.length() + 1;
				if (tokens.back().value.length() == 0)
				{
					std::cout << "Empty double Quotes:" << lineNumber << ":" << i;
					exit(1);
				}
			}
			else if (line[i] == '\'')
			{
				if (line.find('\'', i+1) == std::string::npos)
				{
					std::cout << "Unclosed single Quotes:" << lineNumber << ":" << i;
					exit(1);
				}
				tokens.push_back((t_tokens){
					WORD,
					line.substr(i+1, ((line.find('\'', i + 1) - i) - 1)),
					lineNumber,
					i,
					 });
				i += tokens.back().value.length() + 1;
				i += tokens.back().value.length() + 1;
				if (tokens.back().value.length() == 0)
				{
					std::cout << "Empty single Quotes:" << lineNumber << ":" << i;
					exit(1);
				}
			}
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
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].type == WORD)
		{
			if (SpecialWords.find(tokens[i].value) != SpecialWords.end())
				tokens[i].type = SpecialWords[tokens[i].value];
		}
		if (tokens[i].type != SPACE)
		{
			tokenizedFile.push_back(std::make_pair(tokens[i].type, tokens[i].value));
		}
	}
	return tokenizedFile;
}

bool Tokenizer::is_not_of(char c, std::string str)
{
	if (str.find(c) == std::string::npos)
		return true;
	return false;
}
