#include "../include/PostMethod.hpp"
#include "../include/GetMethod.hpp"
#include "../../Utils/include/DirListing.hpp"
#include <cstdio>
#include <string>

PostMethod::PostMethod(Server &ser, Request &req, Response &res) : ser(ser), req(req), res(res)
{
	LOCATION_ITT it;
	if (isLocation(it)) //--> : is location exist
	{

		if (it->second->isMethodAllowed("POST"))
		{
			if (it->second->isCgiExtention(req.getPath()))
			{
				it->second->getCgiBinFor(req.getPath());
			}
			if (it->second->isUploadOn())
			{
				std::ifstream sourceFile(req.getBody(), std::ios::binary);
				if (!sourceFile) {
					FillResponse(403);
				}
				std::ofstream destinationFile((it->second->getUploadPath() + "/test.jpg"), std::ios::binary);
				if (!destinationFile) {
					sourceFile.close();
					FillResponse(403);
				}
				destinationFile << sourceFile.rdbuf();
				sourceFile.close();
				destinationFile.close();
				if (std::remove(req.getBody().c_str()) != 0) {
					std::perror("Error removing the original file");
				}
				FillResponse(201);
			}
			// else if )
		}
		else
			FillResponse(405);
	}
	else
		FillResponse(404);
	for (std::map<std::string, std::string>::const_iterator it = req.getHeaders().begin(); it != req.getHeaders().end(); it++)
		std::cout << it->first << " : " << it->second << std::endl;
}


void PostMethod::FillResponse(int code)
{
	res.setCode(code);
	res.setMsg(GenerateResponse::generateMsg(res.getCode()));
	res.setHeaderAndStart(GenerateResponse::generateHeaderAndSt(res, req));
}

bool PostMethod::isLocation(LOCATION_ITT &it)
{
	std::string path = req.getPath();
	LOCATION_MAP locations = ser.getAllLocation();
	it = locations.begin();

	for (;it != locations.end(); it++)
	{
		if (it->second->isMatch(path))
			return true;
	}
	return false;
}

PostMethod::~PostMethod(){}
