#pragma once

#include "../../include/includes.hpp"
#include "../../HttpElement/include/Server.hpp"



class GenerateServer{

	public:
		static Server *NewServer(TOKEN_IT &it);

	private:
		static void fillLocation(Server &srv, TOKEN_IT &it);
		static void SetSingleValue(Server &srv, TOKEN_IT &it);
		static void SetMultiValue(Server &srv, TOKEN_IT &it);
		static void SetBodySize(Server &srv, TOKEN_IT &it);
		static void SetTypes(Server &srv, TOKEN_IT &it);
		static void SetErrorPages(Server &srv, TOKEN_IT &it);
		static int parseErrorPage(std::vector<int> &check, std::string codeValue);

		static void SetHostAndPort(Server &srv, TOKEN_IT &it);
		static void parseListen(Server &ser, std::string line);
		static void handelOne(Server &ser, std::string line);
		static void hostV6(Server &ser, std::string line);


};
