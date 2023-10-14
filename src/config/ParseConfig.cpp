#include "../../include/ParseConfig.hpp"
#include <fstream>
#include <sstream>

// Start: Canonical form:

ParseConfig::ParseConfig(){}
ParseConfig &ParseConfig::operator=(ParseConfig const &copy){}
ParseConfig::ParseConfig(ParseConfig const &copy){}

// End: Canonical form


void ParseConfig::FillMimeTypes(std::ifstream &file) {
	std::string	line;
	int			position;
	while (std::getline(file, line))
	{
		if ((position = line.find("include:")) != std::string::npos)
		{
			line = line.substr(position + 9 , (line.find(";") - position - 9));
			break;
		}
	}
	if (line.empty())
		throw std::runtime_error("Error: No include found.");
	std::ifstream mimeFile(line);
	if (!mimeFile.is_open())
		throw std::runtime_error("Error: Mime file not found.");
	bool newMime;
	std::string key;
	std::string value;
	std::stringstream ss;
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


ParseConfig::ParseConfig(std::string &path) {
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error(CONFIG_FILE_NOT_FOUND);
	this->FillMimeTypes(file);
}





