#include "../../include/ParseConfig.hpp"
#include "../../include/TokenConfig.hpp"
// Start: Canonical form:
ParseConfig::ParseConfig(){}
ParseConfig::~ParseConfig(){}
ParseConfig &ParseConfig::operator=(ParseConfig const &copy){
	if (this != &copy){}
	return *this;
}
ParseConfig::ParseConfig(ParseConfig const &copy){
	*this = copy;
}
// End: Canonical form

void ParseConfig::FillMimeTypes(std::ifstream &file) {
	std::string	line;
	int			position;
	while (std::getline(file, line))
	{
		if ((line.find("include:")) != std::string::npos)
		{
			position = line.find("include:");
			line = line.substr(position + 9 , (line.find(";") - position - 9));
			break;
		}
	}
	if (line.empty())
		throw std::runtime_error("Error: No include found.");
	std::ifstream mimeFile(line);
	if (!mimeFile.is_open())
		throw std::runtime_error("Error: Mime file not found.");
	while (std::getline(mimeFile, line))
	{
		if (line.empty())
			continue;
		ss.clear();
		ss.str(line);
		newMime = true;
		while (std::getline(ss, line, ' '))
		{
			if (line.empty())
				continue ;
			if (newMime)
			{
				value = line;
				newMime = false;
			}
			else {
				if (*(line.end() - 1) == ';')
					key = line.substr(0, line.find(';'));
				else
					key = line;
				mimeTypes[key] = value;
			}
		}
	}
}

void ParseConfig::SyntaxError(std::string &fileString){
	(void)fileString;
}

//NOTE::just for print
// void print(token tk){
// 	if (tk == ESP)
// 		std::cout << "espace" << std::endl;
// 	if (tk == SERVER)
// 		std::cout << "server" << std::endl;
// 	if (tk == SEMICOLONE)
// 		std::cout << "SEMICOLONE" << std::endl;
// 	if (tk == COLONE)
// 		std::cout << "COLONE" << std::endl;
// 	if (tk == BRACKETOPEN)
// 		std::cout << "BRACKETOPEN" << std::endl;
// 	if (tk == BRACKETCLOSE)
// 		std::cout << "BRACKETCLOSE" << std::endl;
// 	if (tk == VALUE)
// 		std::cout << "VALUE" << std::endl;
// 	if (tk == INCLUDE)
// 		std::cout << "INCLUDE" << std::endl;
// }

ParseConfig::ParseConfig(std::string &path){

	std::string fileString;
	std::ifstream configFile(path);
	TokenConfig TokenConfig;

	if (!configFile.is_open())	{
		throw std::runtime_error("ERR_OPEN");
	}

	this->tokens = TokenConfig.TokenTheConfig(configFile);
	configFile.close();
	configFile.open(path);
	this->FillMimeTypes(configFile);
	//this->syntaxError(fileString);
	this->generator.Generator(this->tokens, this->servers);

	//NOTE:: Just For Test
	// for (std::vector<TOKEN_PAIR>::iterator it = tokens.begin(); it != tokens.end(); ++it){
	// 	TOKEN_PAIR pair = *it;
	// 	std::cout << pair.first << "   ";
	// 	print(pair.second);
	// }

	configFile.close();
}
