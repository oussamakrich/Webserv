#include "../include/PostMethod.hpp"
#include "../../Utils/include/DirListing.hpp"
#include <string>

PostMethod::PostMethod(Server &ser, Request &req, Response &res) : ser(ser), req(req), res(res)
{
	LOCATION_ITT it;
	if (isLocation(it))
	{
		if (it->second->isMethodAllowed("POST"))
		{
			
		}
		else
		{
			res.setCode(405);
			res.setMsg(GenerateResponse::generateMsg(res.getCode()));
			res.setHeaderAndStart(GenerateResponse::generateHeaderAndSt(res));
		}
	}
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
