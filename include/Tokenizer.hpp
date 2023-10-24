#pragma once
#include "includes.hpp"
#include <vector>

#define																		TOKEN_OUT std::vector<TOKEN>
#define																		TOKEN_STRUCTS std::vector<t_tokens>
#define																		TOKEN_ITERATOR std::vector<t_tokens>::iterator
#define SYNTAX_ERROR														"\033[0;31mSyntax Error: \033[0mline \033[4;37mconfig.conf:"

class Tokenizer {
	private:
		static std::map<std::string, Token> SpecialSymbols;
		static std::map<std::string, Token> SpecialWords;
		static std::stack<Token> curlyBrackets;
		static int serverFound;
	private:
		Tokenizer();
		static void QuotesHandler(std::string &line, int lineNumber, unsigned int &i, TOKEN_STRUCTS &tokens, char quote);
		static void CloseCurlyBracketsSyntax(TOKEN_ITERATOR &i, TOKEN_STRUCTS &tokens, TOKEN_OUT &tokenizedFile);
		static void WordHandler(std::string &line, int lineNumber, unsigned int &i, TOKEN_STRUCTS &tokens);
		static void ServerHandler(TOKEN_OUT &tokenizedFile, TOKEN_STRUCTS &tokens, TOKEN_ITERATOR &i);
		static void BlockHandler(TOKEN_OUT &tokenizedFile, TOKEN_STRUCTS &tokens, TOKEN_ITERATOR &i);
		static void syntaxCheck(TOKEN_OUT &tokenizedFile, TOKEN_STRUCTS &tokens);
		static void fatalError(Error error,TOKEN_ITERATOR &it, std::string str);
		static void SemiColonSyntax(TOKEN_ITERATOR &i, TOKEN_STRUCTS &tokens);
		static void WordSyntax(TOKEN_ITERATOR &i, TOKEN_STRUCTS &tokens);
		static bool is_not_of(char c, std::string str);
		static void validateTokens(TOKEN_STRUCTS &tokens);
		static void generateTokenMap(void);
	public:
		static TOKEN_OUT tokenGenerator(std::ifstream &file);
		~Tokenizer();
};

