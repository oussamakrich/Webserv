#pragma once

#include "includes.hpp"
#include <string>

class Server {
	private:
		short								port; // done.
		int									clientMaxBodySize; // done.
		std::string					host; // done.
		std::string					root; // done.
		std::string					serverName; // done.
		std::vector<std::string>	index; // done.
		std::vector<std::string>	errorPages; // done.
		std::string 				defaultType; // done.
		std::string 				accessLog; // done.
		std::string 				errorLog; // done.
	// std::map<>// mimeType
		// std::map<std::string, Location *>		locations; // done.


	public:
		void SetSingleValue(TOKEN_IT &it);
		void SetMultiValue(TOKEN_IT &it);
		void SetPort(TOKEN_IT &it);
		void SetBodySize(TOKEN_IT &it);

		std::string printvect(std::vector<std::string> vect)
	{
		std::vector<std::string>::iterator it = vect.begin();
		std::string ret;
		for(; it != vect.end(); it++){
			ret += *it;
			ret += "\t";
		}
		return ret;
	}

		void print(){
			std::cout <<"------------------SERVER : " <<  serverName << "---------------" << std::endl;	
			std::cout << "port				: " <<  port << std::endl;
			std::cout << "host				: " <<	host << std::endl;
			std::cout << "root				: "	<<	root<< std::endl;
			std::cout << "defaultType			: "	<<	defaultType << std::endl;
			std::cout << "accessLog			:	"	<<	accessLog << std::endl;
			std::cout << "errorLog			: " <<	errorLog << std::endl;
			std::cout << "index				:	"	<< printvect(index) << std::endl;
			std::cout << "errorPages			:	"	<< printvect(errorPages) << std::endl;
			
		}

		Server();
		~Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);
};



	// public:		// Getters:
	// 	short		getPort() const;
	// 	std::string getErrorLog() const;
	// 	std::string	getHost() const;
	// 	std::string	getServerName() const;
	// 	std::string getRoot() const;
	// 	std::vector<std::string> getIndex() const;
	// 	int			getClientMaxBodySize() const;
	// 	std::vector<std::string> getErrorPages() const;
	// 	std::string getDefaultType() const;
	// 	std::string getAccessLog() const;
	// 	Location const &getLocation(std::string const &url) const;
