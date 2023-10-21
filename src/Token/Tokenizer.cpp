#include "../../include/Tokenizer.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> Tokenizer::allowedMethods = std::vector<std::string>();
std::map<std::string, Token> Tokenizer::SpecialWords = std::map<std::string, Token>();
std::map<std::string, Token> Tokenizer::SpecialSymbols = std::map<std::string, Token>();
std::stack<Token> Tokenizer::curlyBrackets = std::stack<Token>();
std::stack<Token> Tokenizer::squareBrackets = std::stack<Token>();

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
	SpecialWords["host"] = HOST;
	SpecialWords["port"] = PORT;
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

void Tokenizer::fatalError(int error, int line, std::string str)
{
	if (error == 1)
	{
		std::cout << RED"Error:\n" <<  RESET"Line " << line << ": Unexpected token \'" << trim(str) << ".\'\n";
		std::cout << GREEN"                          ^\n";
		std::cout << RESET;
	}
	else if (error == 2)
	{
		std::cout << RED"Error:\n" <<  RESET"Line " << line << ": Unclosed server block." << "\n";
		std::cout << RESET;
	}
	else if (error == 3)
	{
		std::cout << RED"Error:\n" <<  RESET"Line " << line << ": Unclosed directive: " << str << " 'missing semicolon'" << RED" ;" << RESET"\n";
		std::cout << GREEN"                             ^\n";
		std::cout << RESET;
	}
	else if (error == 4)
	{
		std::cout << RED"Error:\n" <<  RESET"Line " << line << ": Unclosed Bracket '{' \'" << trim(str) << ".\'\n";
		std::cout << GREEN"                          ^\n";
		std::cout << RESET;
	}
	std::cout << "See fatal.log for more details.\n";
	std::ofstream fatal("fatal.log");
	if (fatal.is_open() == true)
	{
		fatal << CONFIG_GUID;
		fatal.close();
	}
	exit(1);
}


std::vector<TOKEN> Tokenizer::tokenGenerator(std::ifstream &file)
{
	std::vector<TOKEN> tokens;
	std::stringstream ss;
	std::string line;
	std::string word;
	std::map<std::string,Token>::iterator holder;
	std::map<std::string, Token>::iterator holder2; //

	generateTokenMap();
	int i = 1;
	while (std::getline(file, line))
	{
		ss.clear();
		ss.str(line);
		while (ss>>word)
		{
			word = trim(word);
			holder = SpecialWords.find(word);
			holder2 = SpecialSymbols.find(word);
			if (holder != SpecialWords.end())
			{
				if (serverFound)
					Tokenizer::fatalError(2, i, "");
				if (holder->second == SERVER)
					serverFound = true;
				if (!tokens.empty() && tokens.back().first != SEMICOLON && tokens.back().first != OPEN_C_BRACKET && tokens.back().first != CLOSE_C_BRACKET && tokens.back().first != OPEN_S_BRACKET && tokens.back().first != CLOSE_S_BRACKET)
					Tokenizer::fatalError(3, i - 1, trim(tokens.back().second));
				tokens.push_back(TOKEN(holder->second, word));
				continue;
			}
			else if (word[word.length() - 1] == ';')
			{
				word = word.substr(0, word.length() - 1);
				tokens.push_back(TOKEN(WORD, word));
				tokens.push_back(TOKEN(SEMICOLON, ";"));
				continue;
			}
			else if (holder2 != SpecialSymbols.end())
			{
				if (holder2->second == OPEN_S_BRACKET)
				{
					if (squareBrackets.empty() == false && squareBrackets.top() == OPEN_S_BRACKET)
						Tokenizer::fatalError(4, i, trim(line));
					squareBrackets.push(OPEN_S_BRACKET);
					serverFound = false;
				}
				else if (holder2->second == CLOSE_S_BRACKET)
				{
					if (!squareBrackets.empty() && squareBrackets.top() != OPEN_S_BRACKET)
						Tokenizer::fatalError(4, i, trim(line));
					squareBrackets.pop();
				}
				else if (holder2->second == OPEN_C_BRACKET)
				{
					if (!curlyBrackets.empty() && curlyBrackets.top() == OPEN_C_BRACKET)
						Tokenizer::fatalError(4, i, trim(line));
					curlyBrackets.push(OPEN_C_BRACKET);
				}
				else if (holder2->second == CLOSE_C_BRACKET)
				{
					if (curlyBrackets.empty() || curlyBrackets.top() != OPEN_C_BRACKET)
						Tokenizer::fatalError(1, i, trim(line));
					curlyBrackets.pop();
				}
				tokens.push_back(TOKEN(holder2->second, word));
				continue;
			}
			// else if (tokens.back().first == SEMICOLON)
			// 	Tokenizer::fatalError(1, i, trim(line));
			tokens.push_back(TOKEN(WORD, word));
		}
		i++;
	}

	return tokens;
}
