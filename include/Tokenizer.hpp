#pragma once
#include "includes.hpp"
#include <vector>

#define TOKENIZED_FILE std::vector<TOKEN>
#define TOKEN_STRUCTS std::vector<t_tokens>

class Tokenizer {
	private:
		static std::map<std::string, Token> SpecialWords;
		static std::map<std::string, Token> SpecialSymbols;
		static int serverFound;
		static std::stack<Token> curlyBrackets;
	private:
		static bool is_not_of(char c, std::string str);
		static void WordHandler(std::string &line, int lineNumber, unsigned int &i, std::vector<t_tokens> &tokens);
		static void QuotesHandler(std::string &line, int lineNumber, unsigned int &i, std::vector<t_tokens> &tokens, char quote);
		static void BlockHandler(std::vector<TOKEN> &tokenizedFile, std::vector<t_tokens> &tokens, std::vector<t_tokens>::iterator &i);
		Tokenizer();
		static void fatalError(Error error,std::vector<t_tokens>::iterator &it);
		static bool syntaxCheck(std::vector<TOKEN> &tokenizedFile, std::vector<t_tokens> &tokens);
		static void generateTokenMap(void);
		static void SemiColonSyntax(std::vector<t_tokens>::iterator &i, std::vector<t_tokens> &tokens);
		static void ServerHandler(TOKENIZED_FILE &tokenizedFile, TOKEN_STRUCTS &tokens, std::vector<t_tokens>::iterator &i);
		static void WordSyntax(std::vector<t_tokens>::iterator &i, std::vector<t_tokens> &tokens);
		static void CloseCurlyBracketsSyntax(std::vector<t_tokens>::iterator &i, std::vector<t_tokens> &tokens, std::vector<TOKEN> &tokenizedFile);
	public:
		static std::vector<TOKEN> tokenGenerator(std::ifstream &file);
		~Tokenizer();
};

