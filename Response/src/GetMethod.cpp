
#include "../include/GetMethod.hpp"
#include <cstddef>
#include <fstream>
#include <sys/_types/_size_t.h>

GetMethod::GetMethod(Server &ser, Request &req, Response &res) : ser(ser), req(req), res(res)
{
	GetMethode(ser, req, res);
}

GetMethod::~GetMethod(){}

// void GetMethod::sendReminder(Response &res){
// }

bool GetMethod::isLocation()
{
	std::string path = req.getPath();
	LOCATION_MAP locations = ser.getAllLocation();
	LOCATION_ITT it = locations.begin();
	for (;it != locations.end(); it++)
	{
		if (it->second->isMatch(path))
			return true;
	}
	return false;
}

int GetMethod::isFile(std::string path, size_t &size)
{
	struct stat buffer;
	if (stat(path.c_str(), &buffer) == 0){
		size = buffer.st_size;
		std::cout << "size : " << size << std::endl;
		if (S_ISREG(buffer.st_mode))
			return FILE;
		else if (S_ISDIR(buffer.st_mode))
			return DIRECTORY;
	}
	return NOT_FOUND;
}

std::string findMimeType(std::string path, Server &ser){
	std::string extention = path.substr(path.find_last_of('.') + 1);
	if (extention.empty())
		return ser.getDefaultType();
	TYPES_MAP types = ser.getMimeType();
	TYPES_ITT it = types.find(extention);
	if (it != types.end())
		return it->second;
	return ser.getDefaultType();
}

void GetMethod::serveFile(std::string path, size_t size){
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
		std::cout << findMimeType(path, ser) << std::endl;
		res.setCode(200);
		}
		else{
			res.setCode(404);
		}
}

void GetMethod::simpleGet(){
	std::string path = ser.getRoot() + '/' + req.getPath();
	size_t size;
	int type = isFile(path, size);
	if (type == FILE){ // TODO ::Read the file and serv it 
		res.path = path;
		serveFile(path, size);
	}
	else if (type == DIRECTORY){ //TODO :  try index.html || check auto index
	}
	else if (type == NOT_FOUND) {//TODO : Generate 404
		// res.setCode(404);
	}
}

void GetMethod::GetMethode(Server &ser, Request &req, Response &res)
{
	if (isLocation()){
		std::cout << "locationGet" << std::endl;
		// locationGet();
	}
	else
		simpleGet();
}
