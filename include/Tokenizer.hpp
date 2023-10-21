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
		static void QuotesHandler(std::string &line, int lineNumber, unsigned int &i, std::vector<t_tokens> &tokens, char quote);
		static void BlockHandler(std::vector<TOKEN> &tokenizedFile, std::vector<t_tokens> &tokens, std::vector<t_tokens>::iterator &i);
		Tokenizer();
		Tokenizer(const Tokenizer &copy);
		static void fatalError(Error error,std::vector<t_tokens>::iterator &it);
		static bool syntaxCheck(std::vector<TOKEN> &tokenizedFile, std::vector<t_tokens> &tokens);
		static void generateTokenMap(void);
		static void SemiColonSyntax(std::vector<t_tokens>::iterator &i, std::vector<t_tokens> &tokens);
		// static void tokenizeSpecialSymbols(std::string &line, int lineNumber, int index);
	public:
		static std::vector<TOKEN> tokenGenerator(std::ifstream &file);
		~Tokenizer();
};
