#include "../include/PostMethod.hpp"
#include "../../Utils/include/DirListing.hpp"
#include <string>

PostMethod::PostMethod(Server &ser, Request &req, Response &res) : ser(ser), req(req), res(res)
{
	// LOCATION_ITT it;
	// if (isLocation(it)) //--> : is location exist
	// {
	// 	if (it->second->isMethodAllowed("POST"))
	// 	{
	// 		if (it->second->isUploadOn())
	// 		{
	// 			std::string path = req.getBody();
	// 			req.getHeader()
	// 		}
	// 		else
	// 			FillResponse(403);
	// 	}
	// 	else
	// 		FillResponse(405);
	// }
	// else
	// 	FillResponse(404);
	// print all the headers.
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
