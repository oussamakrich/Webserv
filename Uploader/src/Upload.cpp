#include "../include/Upload.hpp"
#include <fstream>
#include <cstdio>
Upload::Upload(Server &ser, Request &req, Response &res, Location &loc):output_offset(0), uploadSize(520), input_offset(0)
{
	Uploader(ser, req, res, loc);
}


std::string Upload::GetFileName(Request &req)
{

	if ((req.getHeader("Content-Disposition")).empty())
	{
		std::string path = req.getPath();
		size_t pos = path.find_last_of('/');
		if (pos == std::string::npos)
			return path;
		return path.substr(pos + 1);
	}
	std::string disposition = req.getHeader("Content-Disposition");

	size_t pos = disposition.find("filename=");
	if (pos == std::string::npos)
	{
		std::string path = req.getPath();
		size_t pos = path.find_last_of('/');
		if (pos == std::string::npos)
			return path;
		return path.substr(pos + 1);
	}
	pos += 10;
	size_t end = disposition.find(";", pos);
	if (end == std::string::npos)
		return disposition.substr(pos);
	return disposition.substr(pos, end - pos);
}

void Upload::multipart(Server &ser, Request &req, Response &res, Location &loc)
{
	std::string boundary;
	std::ifstream file(req.getBody().c_str(), std::ios::binary);

	std::getline(file, boundary);
	std::cout << "boundary: " << boundary << std::endl;

}

void Upload::Uploader(Server &ser, Request &req, Response &res, Location &loc)
{
	std::string sourceFile = req.getBody();
	std::string destinationFile = loc.getUri() + loc.getUploadPath() + GetFileName(req);

	// if (req.getHeader("Content-Type").find("multipart/form-data") != std::string::npos)
	// {
	// 	multipart(ser, req, res, loc);
	// }
	// else
	// {
	std::cout << destinationFile << std::endl;
		if (rename(sourceFile.c_str(), destinationFile.c_str()) == 0)
		{
			std::cout<< GREEN"File Uploaded successfully: " <<U_WHITE<< destinationFile << "\n";
			res.setCode(201);
			res.stillSend = false;
		}
		else
		{
			std::cout << YELLOW"Warning:"<<  RESET" unable to upload the file\n";
			res.setCode(500);
			res.stillSend = false;
		}
	// }

}

Upload::~Upload(){}
