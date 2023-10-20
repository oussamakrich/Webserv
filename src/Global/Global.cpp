#include "../../include/Global.hpp"
#include <exception>
#include <fstream>
#include <sstream>
#include <stdexcept>

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

	MimeStream << MimeFile.rdbuf();
	MimeFile.close();
	
}

void	Global::FillMimeType(std::stringstream &MimeStream){
		
}

void Global::addServer(Server *server){
	this->servers.push_back(server);
}
