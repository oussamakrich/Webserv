
#include "../include/GetMethod.hpp"
#include "../../Utils/include/DirListing.hpp"
#include <string>

GetMethod::GetMethod(Server &ser, Request &req, Response &res) : ser(ser), req(req), res(res)
{
	GetMethode(ser, req, res);
}

GetMethod::~GetMethod(){}

bool GetMethod::isLocation(LOCATION_ITT &it)
{
	std::string path = req.getPath();
	LOCATION_MAP &locations = ser.getAllLocation();
	it = locations.begin();

	for (;it != locations.end(); it++)
		if (it->second->isMatch(path))
			return true;
	return false;
}

int GetMethod::isFile(std::string path, size_t &size)
{
	struct stat buffer;

	if (stat(path.c_str(), &buffer) == 0){
		size = buffer.st_size;
		if (S_ISREG(buffer.st_mode))
			return FILE;
		else if (S_ISDIR(buffer.st_mode))
			return DIRECTORY;
	}
	return NOT_FOUND;
}

std::string GetMethod::findMimeType(std::string path, Server &ser){
	std::string extention = path.substr(path.find_last_of('.') + 1);
	if (extention.empty())
		return defaultType;
	TYPES_MAP types = ser.getMimeType();
	TYPES_ITT it = types.find(extention);
	if (it != types.end())
		return it->second;
	return defaultType;
}

void copy(char *buffer, std::string out){
	for (size_t i = 0; i < out.size(); i++)
		buffer[i] = out[i];
}

void GetMethod::serveDirectory(){
	size_t size;
	int type;
	std::vector<std::string>::iterator it = indexes.begin();
	for (; it != indexes.end(); it++){
		std::string path = res.path + '/' + *it;
		type = isFile(path, size);
		if (type == FILE){
			serveFile(path, size);
			return;
		}
	}
	if (autoindex){ //TODO : List Dir if auto index on
		std::string output;
		if (DirListing::getDirlistigHtml(res.path, output)){
			char *buffer = new char[output.size()];
			copy(buffer , output);
			res.setBuffer(buffer, output.size());
			res.stillSend = false;
			res.setHeadr("Content-Length: " + convertCode(output.size()));
			res.setHeadr("Content-Type: text/html");
			res.setCode(200);
		}
		else
			res.setCode(403);
	}
	else{
		res.setCode(403);
	}
}

void GetMethod::serveFile(std::string path, size_t size){

		if (checkCGI()){
			// handelCgi();
			return;
		}
		std::ifstream file(path.c_str());

		if (file.is_open()){
			char *buffer = new char[R_READ];
			file.read(buffer, R_READ);
			res.setBuffer(buffer, file.gcount());	
			res.pos = file.gcount();
			res.stillSend = true;
			if (file.eof())
				res.stillSend = false;
			file.close();
		res.setHeadr("Content-Length: " + convertCode(size));
		res.setHeadr("Content-Type: " + findMimeType(path, ser));
		res.setCode(200);
		}
		else{
			res.setCode(404);
		}
}

bool GetMethod::checkCGI(){
	if (isLoacation)
		// return location->isCGI();
		return false;
	else
		return false;
}

void GetMethod::simpleGet(){
	res.path = this->root + '/' + req.getPath();
	size_t size;
	int type = isFile(res.path, size);

	if (type == FILE)
		serveFile(res.path, size);
	else if (type == DIRECTORY) //TODO :  try index.html || check auto index
		serveDirectory();
	else if (type == NOT_FOUND) //TODO : Generate 404
		res.setCode(404);
	
}

bool GetMethod::checkRedirectionAndAllowed(){
	if (!location->isMethodAllowed(req.getMethod())){ // NOTE :Check if method allowed or Not
		res.setCode(405);
		return true;
	}
	if (location->isRedirection()){
		// res.setCode(location->getRedirectionCode());
		// TODO : Redirection if betwen 100 200 400  -> text html else -> header location 
		// if (res.getCode() == 200)
		
		return true;
	}
	return false;
}

void GetMethod::GetMethode(Server &ser, Request &req, Response &res)
{
	LOCATION_ITT it;

	if (isLocation(it)){
		isLoacation = true;
		location = it->second;
		if (checkRedirectionAndAllowed())
			return;
		autoindex = location->isAutoIndex();
		root = location->getRoot();
		indexes = location->getIndexesList();
		defaultType = location->getDefaultTypes();
	}
	else{
		isLoacation = false;
		autoindex = ser.getAutoIndex();
		root = ser.getRoot();
		indexes = ser.getIndex();
		defaultType = ser.getDefaultType();
	}
	indexes.push_back("index.html");
	simpleGet();
}
