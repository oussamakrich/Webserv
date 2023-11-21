#include "../include/Tokenizer.hpp"

std::map<std::string, Token> Tokenizer::SpecialWords = std::map<std::string, Token>();
std::map<std::string, Token> Tokenizer::SpecialSymbols = std::map<std::string, Token>();
std::stack<Token> Tokenizer::curlyBrackets = std::stack<Token>();
int Tokenizer::serverFound = 0;
std::string Tokenizer::fileName = "";

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
	SpecialWords["autoindex"] = AUTO_INDEX;
	SpecialWords["allowed_methods"] = ALLOWED_METHOD;
	SpecialWords["upload"] = UPLOAD;
	SpecialWords["download"] = DOWNLOAD;
	SpecialWords["upload_path"] = UPLOAD_PATH;
	SpecialWords["cgi"] = CGI;
	SpecialSymbols["{"] = OPEN_C_BRACKET;
	SpecialSymbols["}"] = CLOSE_C_BRACKET;
	SpecialSymbols[";"] = SEMICOLON;
}

void Tokenizer::validateTokens(TOKEN_STRUCTS &tokens)
{
	unsigned int size = tokens.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (i < size && tokens[i].type == SEMICOLON)
		{
			i++;
			while (i < size && tokens[i].type == SPACE)
				i++;
			if (i < size && (tokens[i].type == SEMICOLON || tokens[i].type == OPEN_C_BRACKET))
			{
				std::cerr << SYNTAX_ERROR <<fileName << ":" << tokens[i].column << ":" << tokens[i].row << RESET" expected word or quotes after semicolon.\n";
				std::cerr << RED"\n\t\tError code: ("<< (MISSING_SEMICOLON + 1) <<  ")" << RESET <<"\n";
				exit(1);
			}
		}
		else if (i < size && tokens[i].type == OPEN_C_BRACKET)
		{
			i++;
			while (i < size && tokens[i].type == SPACE)
				i++;
			if (i < size && tokens[i].type == CLOSE_C_BRACKET)
			{
				std::cerr << SYNTAX_ERROR  <<fileName << ":" <<  tokens[i].column << ":" << tokens[i].row << RESET" empty blocks are not allowed.\n";
				std::cerr << RED"\n\t\tError code: ("<< (EMPTY_BLOCK + 1)  <<  ")" << RESET <<"\n";
				exit(1);
			}
			if (i < size && tokens[i].type == SEMICOLON)
			{
				std::cerr << SYNTAX_ERROR  <<fileName << ":" <<  tokens[i].column << ":" << tokens[i].row << RESET" expected word or quotes before semicolon.\n";
				std::cerr << RED"\n\t\tError code: ("<< (MISSING_SEMICOLON + 1)  <<  ")" << RESET <<"\n";
				exit(1);
			}
		}

	}
}

void Tokenizer::WordHandler(std::string &line, int lineNumber, unsigned int &i, TOKEN_STRUCTS &tokens)
{
	unsigned int len = i;
	while (len < line.length() && Tokenizer::is_not_of(line[len], " \t\r;{}#"))
		len++;
	tokens.push_back((t_tokens){
		WORD,
		line.substr(i, (len - i)),
		lineNumber,
		i
	});
	i += (len - i) - 1;
}

void Tokenizer::QuotesHandler(std::string &line, int lineNumber, unsigned int  &i, TOKEN_STRUCTS &tokens, char quote)
{
	if (line.find(quote, i + 1) == std::string::npos)
	{
		if (quote == '\"') std::cerr << "Unclosed Double Quotes: line: " << U_WHITE << fileName<<":" << lineNumber << ":" << i;
		else std::cerr << "Unclosed Single Quotes: line: " << U_WHITE<<fileName << ":" << lineNumber << ":" << i;
		exit(1);
	}
	tokens.push_back((t_tokens){
		QUOTES,
		line.substr(i+1, ((line.find(quote, i + 1) - i) - 1)),
		lineNumber,
		i,
		 });
	i += (tokens.back().value.length() + 1);
	if (tokens.back().value.empty())
	{
		if (quote == '\"')
			std::cerr << SYNTAX_ERROR  <<fileName << ":" <<  lineNumber << ":" << i << RESET" Empty Double Quotes. \n";
		else
			std::cerr << SYNTAX_ERROR  <<fileName << ":" <<  lineNumber << ":" << i << RESET" Empty Single Quotes. \n";
		std::cerr << RED"\n\t\tError code: ("<< (EMPTY_QUOTES + 1) << ")\n" << RESET;
		exit(1);
	}
}

TOKEN_OUT Tokenizer::tokenGenerator(std::ifstream &file, std::string filename)
{
	TOKEN_STRUCTS	tokens;
	TOKEN_OUT 	tokenizedFile;
	int					lineNumber = 1;
	std::string			line;
	Tokenizer::fileName = filename;

	Tokenizer::generateTokenMap();
	while (std::getline(file, line))
	{
		for (unsigned int i = 0; i < line.length(); i++)
		{
			if (std::isspace(line[i])) tokens.push_back((t_tokens){SPACE, " ", lineNumber, i});
			else if (line[i] == '{') tokens.push_back((t_tokens){OPEN_C_BRACKET, "{", lineNumber, i});
			else if (line[i] == '}') tokens.push_back((t_tokens){CLOSE_C_BRACKET, "}", lineNumber, i});
			else if (line[i] == ';') tokens.push_back((t_tokens){SEMICOLON, ";", lineNumber, i});
			else if (line[i] == '\"' || line[i] == '\'') Tokenizer::QuotesHandler(line, lineNumber, i, tokens, line[i]);
			else if (line[i] == '#')
				break;
			else Tokenizer::WordHandler(line, lineNumber, i, tokens);
		}
		lineNumber++;
	}
	Tokenizer::validateTokens(tokens);
	Tokenizer::syntaxCheck(tokenizedFile, tokens);
	return tokenizedFile;
}

void Tokenizer::SemiColonSyntax(TOKEN_STRUCTS::iterator &i, TOKEN_STRUCTS &tokens)
{
	if (i == tokens.begin())
		Tokenizer::fatalError(DIRECTIVE_SYNTAX_ERROR, i, RESET" expected space after ");
	TOKEN_ITERATOR it = i;
	it--;
	while (it != tokens.begin() && it->type == SPACE)
		it--;
	if (it != tokens.begin() && it->type != SEMICOLON && it->type != OPEN_C_BRACKET && it->type != CLOSE_C_BRACKET)
		Tokenizer::fatalError(MISSING_SEMICOLON, it, RESET " missing semicolon after ");
}

void Tokenizer::BlockHandler(TOKEN_OUT &tokenizedFile, TOKEN_STRUCTS &tokens, TOKEN_ITERATOR &i)
{
	bool isLocation = (i->type == LOCATION);

	TOKEN_ITERATOR type = i;
	i++;
	while (i != tokens.end() && i->type == SPACE) i++;
	if ((i == tokens.end() || (i->type != WORD && i->type != QUOTES)) && isLocation)
		Tokenizer::fatalError(MISSING_URL_BLOCK, type,  RESET " missing url ");
	if (i->type == QUOTES) i->type = WORD;
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
	if (isLocation) i++;

	while (i != tokens.end() && i->type == SPACE) i++;
	if (i == tokens.end() || i->type != OPEN_C_BRACKET)
		Tokenizer::fatalError(UNCLOSED_BRACKETS, type, RESET " unclosed or missing open bracket.");
	if (isLocation && i->type == OPEN_C_BRACKET)
		tokenizedFile.push_back(std::make_pair(i->type, i->value));
	i++;
	while (i != tokens.end() && i->type != CLOSE_C_BRACKET)
	{
		if (i->type == OPEN_C_BRACKET)
			Tokenizer::fatalError(NESTED_BLOCKS_NOT_ALLOWED, type, RESET " nested blocks are not allowed.");
		else if (i->type == WORD && SpecialWords.find(i->value) != SpecialWords.end())
		{
			tokenizedFile.push_back(std::make_pair(SpecialWords.find(i->value)->second, i->value));
			Tokenizer::SemiColonSyntax(i, tokens);
		}
		else if (i->type != SPACE)
		{
			if (i->type == QUOTES) i->type = WORD;
			tokenizedFile.push_back(std::make_pair(i->type, i->value));
			if (i->type == WORD && !isLocation && i->value.find('/') != std::string::npos)
				Tokenizer::SemiColonSyntax(i, tokens);
		}
		i++;
	}
	if (i == tokens.end())
		Tokenizer::fatalError(UNCLOSED_BRACKETS, type, RESET " unclosed or missing open bracket.");
	Tokenizer::SemiColonSyntax(i, tokens);
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
}

void Tokenizer::ServerHandler(TOKEN_OUT &tokenizedFile, TOKEN_STRUCTS &tokens, TOKEN_ITERATOR &i)
{

	Tokenizer::serverFound++;
	i++;
	while (i != tokens.end() && i->type == SPACE)
		i++;
	if (i == tokens.end() || i->type != OPEN_C_BRACKET)
		Tokenizer::fatalError(UNCLOSED_BRACKETS, --i, RESET " unclosed or missing open bracket.");
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
	Tokenizer::curlyBrackets.push(OPEN_C_BRACKET);
}


void Tokenizer::WordSyntax(TOKEN_ITERATOR &i, TOKEN_STRUCTS &tokens)
{
	if (i->type == QUOTES) i->type = WORD;
	if (i->type == WORD)
	{
		TOKEN_ITERATOR it = i;
		if (i == tokens.begin())
			Tokenizer::fatalError(UNKNOWN_DIRECTIVE, i, RESET " unknown directive ");
		it--;
		while (it != tokens.begin() && it->type == SPACE)
			it--;
		if (it != tokens.begin() && (it->type == SEMICOLON || it->type == CLOSE_C_BRACKET || it->type == OPEN_C_BRACKET))
			Tokenizer::fatalError(UNKNOWN_DIRECTIVE, i, RESET " unknown directive ");
	}
}

void Tokenizer::CloseCurlyBracketsSyntax(TOKEN_ITERATOR &i, TOKEN_STRUCTS &tokens, TOKEN_OUT &tokenizedFile)
{
	Tokenizer::SemiColonSyntax(i, tokens);
	if (!Tokenizer::curlyBrackets.empty())
		Tokenizer::curlyBrackets.pop();
	else
		Tokenizer::fatalError(UNCLOSED_BRACKETS, i, RESET " unclosed or missing open bracket.");
	TOKEN_ITERATOR it = i;
	tokenizedFile.push_back(std::make_pair(i->type, i->value));
	it++;
	while (it != tokens.end() && it->type == SPACE)
		it++;
	if (it != tokens.end() && SpecialWords[it->value] != SERVER)
		Tokenizer::fatalError(UNCLOSED_BRACKETS, i, RESET " unclosed or missing open bracket.");
}


void Tokenizer::syntaxCheck(TOKEN_OUT &tokenizedFile, TOKEN_STRUCTS &tokens)
{
	TOKEN_ITERATOR i;

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
					Tokenizer::fatalError(UNCLOSED_BRACKETS, i, RESET " unclosed or missing open bracket.");
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
	if (Tokenizer::serverFound == 0) Tokenizer::fatalError(SERVER_BLOCK_NOT_FOUND, i, RESET " server block not found.\n\n");
	else if (!Tokenizer::curlyBrackets.empty()) Tokenizer::fatalError(UNCLOSED_BRACKETS, --i, RESET " unclosed or missing open bracket.");
}

bool Tokenizer::is_not_of(char c, std::string str) {return (str.find(c) == std::string::npos);}


void Tokenizer::fatalError(Error error,TOKEN_ITERATOR &i, std::string str)
{
	if (error == SERVER_BLOCK_NOT_FOUND)
	{
		std::cerr << SYNTAX_ERROR  <<fileName << ":" <<  0 << ":" << 0 << str ;
		std::cerr << RED"\t\tError code: ("<< (error + 1) << ")\n" << RESET;
	}
	else
	{
		std::cerr << SYNTAX_ERROR  <<fileName << ":" <<  i->column << ":" << (i)->row << str << i->value << ".\n\n" ;
		std::cerr << RED"\t\tError code: ("<< (error + 1) << ")\n" << RESET;
	}
	exit(error + 1);
}
