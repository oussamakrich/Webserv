
#include "../include/AllMethod.hpp"
#include "../../Utils/include/DirListing.hpp"
#include "../../Uploader/include/Upload.hpp"

ResponseHandler::ResponseHandler(Server &ser, Request &req, Response &res) : ser(ser), req(req), res(res)
{
	ResponseHandlere(ser, req, res);
}

ResponseHandler::~ResponseHandler(){}

bool ResponseHandler::isLocation(LOCATION_ITT &it)
{
	std::string path = req.getPath();
	LOCATION_MAP &locations = ser.getAllLocation();
	it = locations.begin();

	for (;it != locations.end(); it++)
		if (it->second->isMatch(path))
			return true;
	return false;
}

int isFile(std::string path, size_t &size)
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

std::string ResponseHandler::findMimeType(std::string path, Server &ser){
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

void ResponseHandler::serveDirectory(){
	size_t size;
	int type;

	if (req.getMethod() == "DELETE") {//NOTE : req is DELETE
		res.setCode(403);
		return;
	}
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
		if (DirListing::getDirlistigHtml(res.path, output, req.getPath())){
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


bool ResponseHandler::checkCGI(){
	if (isLoacation){
		return location->isCgiExtention(res.path);
	}
	else
		return false;
}

void ResponseHandler::handelCGI(){

	if (req.getMethod() == "DELETE") {//NOTE : req is DELETE  && iscgi == Not Implemented
		res.setCode(501);
		return;
	}
	std::string bin = location->getCgiBinFor(res.path);
	res.cgiInfo =  Cgi::Run(req, bin, res.path);
	if (res.cgiInfo.code == 500){
		res.setCode(500);
		return;
	}
	res.isCGI = true;
}

void ResponseHandler::serveFile(std::string path, size_t size){

	res.isCGI = false;
	if (checkCGI()){
		handelCGI();
		return;
	}
	if (req.getMethod() == "DELETE") {//NOTE : req is DELETE
		if (unlink(path.c_str()) == 0)
			res.setCode(200);//NOTE : No Content???
		else
			res.setCode(500); //NOTE : check permission
		return;
	}
	std::ifstream file(path.c_str());

	if (!file.is_open()){
		res.setCode(500);
		return;
	}
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


void ResponseHandler::simpleGet(){
	res.path = this->root + '/' + req.getPath();
	removeSlash(res.path);
	size_t size;
	int type = isFile(res.path, size);

	if (req.getMethod() == "POST" && isLoacation && location->isUploadOn()) { //TODO : check Cgi
		Upload up(ser, req, res, *location);
		return;
	}
	if (type == FILE)
		serveFile(res.path, size);
	else if (type == DIRECTORY) //TODO :  try index.html || check auto index
		serveDirectory();
	else if (type == NOT_FOUND) //TODO : Generate 404
		res.setCode(404);

}

bool ResponseHandler::checkRedirection(){
	char *buffer;
	if (location->isRedirection()){
		res.setCode(location->getRedirectionCode());
		if (res.getCode() >= 300 && res.getCode() < 400){
			res.setHeadr("Location: " + location->getRedirectionText());
		}
		else {
			res.setHeadr("Content-Length: " + convertCode(location->getRedirectionText().size()));
			res.setHeadr("Content-Type: text/html");
			buffer = new char[location->getRedirectionText().size()];
			copy(buffer, location->getRedirectionText());
			res.setBuffer(buffer, location->getRedirectionText().size());
		}
		return true;
	}
	return false;
}

void ResponseHandler::ResponseHandlere(Server &ser, Request &req, Response &res)
{
	LOCATION_ITT it;

	if (isLocation(it)) {
		isLoacation = true;
		location = it->second;
		if (checkRedirection())
			return;
		if (!location->isMethodAllowed(req.getMethod())){
			res.setCode(405);
			return ;
		}
		defaultType = location->getDefaultTypes();
		autoindex = location->isAutoIndex();
		root = location->getRoot();
		indexes = location->getIndexesList();
		res.errorPage = location->getErrorPageList();
	}
	else
	{
		isLoacation = false;
		autoindex = ser.getAutoIndex();
		root = ser.getRoot();
		indexes = ser.getIndex();
		defaultType = ser.getDefaultType();
		res.errorPage = ser.getErrorPages();
	}
	indexes.push_back("index.html");
	simpleGet();
}
