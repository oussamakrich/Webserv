#pragma once

#include "Client.hpp"
#include "../../include/includes.hpp"
#include "Location.hpp"
#include <sys/poll.h>

#define str_it std::vector<std::string>::iterator
#define LOCATION_PAIR std::pair<std::string, Location*>
#define LOCATION_MAP std::map<std::string, Location*>
#define TYPES_MAP std::map<std::string, std::string>
#define TYPES_PAIR std::pair<std::string, std::string>
#define VECT_STR std::vector<std::string>
#define VECT_ERRORPIR std::vector<ERRPAGE_PAIR>
#define ITT_ERRORPIR std::vector<ERRPAGE_PAIR>::iterator
#define VECT_CLIENT std::vector<Client*>
#define ITT_CLIENT	std::vector<Client*>::iterator

class Server {

	public:
		Server();
		~Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);

	private:
		int									port; // done.
		int    							clientMaxBodySize; // done.
		std::string					host; // done.
		std::string					root; // done.
		std::string					serverName; // done.
		VECT_STR						index; // done.
		VECT_ERRORPIR				errorPages; // done.
		std::string 				defaultType; // done.
		std::string 				accessLog; // done.
		std::string					errorLog; // done.
		TYPES_MAP						mimeType;
		LOCATION_MAP				locations; // done.

		int									_listen;
		VECT_CLIENT					clients;


		public:
			void setPort(int port);
		  void setClientMaxBodySize(int size);
		  void setHost(const std::string& host);
		  void setRoot(const std::string& root);
		  void setServerName(const std::string& name);
		  void setIndex(const std::vector<std::string>& index);
		  void setErrorPages(const VECT_ERRORPIR& errorPages);
		  void setErrorPage(const	 ERRPAGE_PAIR errorPage);
		  void setDefaultType(const std::string& defaultType);
		  void setAccessLog(const std::string& accessLog);
		  void setErrorLog(const std::string& errorLog);
		  void setMimeType(const TYPES_MAP& mimeType);
		  bool setMimeType(const TYPES_PAIR& mimeType);
		  void setLocations(const LOCATION_MAP& locations);
		  void setSingleLocation(const LOCATION_PAIR& location);
		  void setItIndex(str_it begin, str_it end);

	public:
		std::vector<int>		CheckRepeat;


		void final();

		bool	 start();
		bool	 handelFd(struct pollfd fd);
		void	 acceptClient();
		bool	 handelClient(ITT_CLIENT it);
		ITT_CLIENT	findClient(pollfd pfd);
		

	public:
		int	getPort() const;
		int	getClientMaxBodySize() const;
		int	getListen() const;

		std::string getErrorLog() const;
		std::string	getHost() const;
		std::string	getServerName() const;
		std::string getRoot() const;
		std::string getDefaultType() const;
		std::string getAccessLog() const;

		std::vector<std::string> getIndex() const;
		std::vector<ERRPAGE_PAIR> getErrorPages() const;
		std::map<std::string, Location*> getAllLocation() const;
		std::map<std::string, std::string> getMimeType() const;
		Location	&getLocation(std::string url);
};


std::ostream &operator<<(std::ostream &out, const Server &server);
