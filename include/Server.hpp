#pragma once

#include "includes.hpp"
#include <string>

class Server {

	public:
		Server();
		~Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);

	private:
		int								port; // done.
		int								clientMaxBodySize; // done.
		std::string					host; // done.
		std::string					root; // done.
		std::string					serverName; // done.
		std::vector<std::string>	index; // done.
		std::vector<std::string>	errorPages; // done.
		std::string 				defaultType; // done.
		std::string 				accessLog; // done.
		std::string 				errorLog; // done.
		std::map<std::string, std::string> mimeType;
		// std::map<std::string, Location *>		locations; // done.


	public:
		void SetSingleValue(TOKEN_IT &it);
		void SetMultiValue(TOKEN_IT &it);
		void SetInt(TOKEN_IT &it);
		void SetTypes(TOKEN_IT &it);
		void SetHostAndPort(TOKEN_IT &it);


	//FIX : Just for print 
		void print();

	public:
		int			getPort() const;
		int			getClientMaxBodySize() const;
		std::string getErrorLog() const;
		std::string	getHost() const;
		std::string	getServerName() const;
		std::string getRoot() const;
		std::vector<std::string> getIndex() const;
		std::vector<std::string> getErrorPages() const;
		std::string getDefaultType() const;
		std::string getAccessLog() const;


};



	// public:		// Getters:
	// 	Location const &getLocation(std::string const &url) const;
