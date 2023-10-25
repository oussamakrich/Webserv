#pragma once

#include "Client.hpp"
#include "includes.hpp"
#include "Location.hpp"
#include <ostream>
#include <sys/poll.h>

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
		std::vector<ERRPAGE_PAIR>	errorPages; // done.
		std::string 				defaultType; // done.
		std::string 				accessLog; // done.
		std::string					errorLog; // done.
		std::map<std::string, std::string>	mimeType;
		std::map<std::string, Location*>		locations; // done.
		std::vector<int> CheckRepeat;

		int	_listen;

		std::vector<Client *> clients;	



	public:
		void fillLocation(TOKEN_IT &it);
		void SetSingleValue(TOKEN_IT &it);
		void SetMultiValue(TOKEN_IT &it);
		void SetInt(TOKEN_IT &it);
		void SetTypes(TOKEN_IT &it);
		void SetHostAndPort(TOKEN_IT &it);
		void SetErrorPages(TOKEN_IT &it);
		void Shrink();

		bool	 start();
		bool	 isMyFd(int fd);
		void	 handelFd(struct pollfd fd);


	private:
		void parseListen(std::string line);
		void handelOne(std::string line);
		void hostV6(std::string line);
		int parseErrorPage(std::string codeValue);


	public:
		int			getPort() const;
		int			getClientMaxBodySize() const;
		std::string getErrorLog() const;
		std::string	getHost() const;
		std::string	getServerName() const;
		std::string getRoot() const;
		std::vector<std::string> getIndex() const;
		std::vector<ERRPAGE_PAIR> getErrorPages() const;
		std::string getDefaultType() const;
		std::string getAccessLog() const;
		Location	&getLocation(std::string url);
		std::map<std::string, Location*> getAllLocation() const;
		std::map<std::string, std::string> getMimeType() const;

		int	getListen() const;

};



	// public:		// Getters:

std::ostream &operator<<(std::ostream &out, const Server &server);
