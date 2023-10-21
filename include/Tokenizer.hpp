#pragma once
#include "includes.hpp"
#include <vector>

class Tokenizer {
	private:
		static std::vector<std::string> allowedMethods;
		static std::map<std::string, Token> SpecialWords;
		static std::map<std::string, Token> SpecialSymbols;
		static bool serverFound;
		static std::stack<std::pair<Token, int> > curlyBrackets;
		static std::stack<std::pair<Token, int> > squareBrackets;
	private:
		static bool is_not_of(char c, std::string str);
		// static void checkForBrackets(std::map<std::string, Token>::iterator it, int lineNumber, std::string &line);
		Tokenizer();
		Tokenizer(const Tokenizer &copy);
		// static void fatalError(int error, int line, std::string str);
		// static bool syntaxCheck();
		static void generateTokenMap(void);
		// static void tokenizeSpecialSymbols(std::string &line, int lineNumber, int index);
	public:
		static std::vector<TOKEN> tokenGenerator(std::ifstream &file);
		~Tokenizer();
};
