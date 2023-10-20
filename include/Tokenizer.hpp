#pragma once
#include "includes.hpp"
#include <vector>

class Tokenizer {

	private:
		Tokenizer();
		Tokenizer(const Tokenizer &copy);
		static std::vector<std::string> allowedMethods;
		static std::map<std::string, Token> SpecialWords;
		static std::map<std::string, Token> SpecialSymbols;
		static void fatalError(int error, int line, std::string str);
		static bool syntaxCheck();
		static bool serverFound;
		static std::stack<Token> curlyBrackets;
		static std::stack<Token> squareBrackets;
	public:
		static void generateTokenMap(void);
		static std::vector<TOKEN> tokenGenerator(std::ifstream &file);
		~Tokenizer();
};
