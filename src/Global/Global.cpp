#include "../../include/Global.hpp"
#include <exception>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

Global::Global(){

}
Global::~Global(){}

Global::Global(const Global &copy){*this = copy;}

Global &Global::operator=(const Global &copy){
	servers = copy.servers;
	mime_types = copy.mime_types;
	return *this;
}

void	Global::HandelMimeTypes(TOKEN_IT &it){
	if (it->first != WORD){
			std::cerr << "Error: include should be followed by Word" << std::endl;
			return;
	}
	std::string mimeStr = it->second;		
	try {
		this->openMimeFile(mimeStr);
	}
	catch(std::exception &e){
			std::cerr << e.what() << std::endl;
	}
}

void	Global::openMimeFile(std::string Mimestr){

	std::ifstream MimeFile(Mimestr);
	std::stringstream MimeStream; 

	if (!MimeFile.is_open())
		throw std::runtime_error("Error: error in MimeType File");

	std::string line;
	while (!MimeFile.eof())
	{
		std::getline(MimeFile, line);	
		Otrim(line);
		if (line.empty())
			continue;
		if (line.back() != ';')
		{
			MimeFile.close();
			throw std::runtime_error("Error: in " + Mimestr + " evry line should end with ';'");
		}
		try{
			this->FillMimeType(line);
		}
		catch (std::exception &e){
			MimeFile.close();
			std::cerr << e.what() << std::endl;
			return;	
		}
	}
	MimeFile.close();
}

void	Global::FillMimeType(std::string line)
{
	std::stringstream FillStream(line);
	std::string key, value;
	
	FillStream >> value;
	FillStream >> key;
	if (key.back() != ';')
		throw std::runtime_error("Error : in MIMETYPE file accept only KEY VALUE;");
	key.erase(key.size() - 1);
	this->mime_types.insert(std::make_pair(key, value));
}

void Global::addServer(Server *server){
	this->servers.push_back(server);
}
