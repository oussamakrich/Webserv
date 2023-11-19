#pragma once

#include "../../include/includes.hpp"
#include "Location.hpp"
#include "Client.hpp"
#include <functional>
#include <string>
#include <sys/_types/_size_t.h>


#define str_it std::vector<std::string>::iterator
#define LOCATION_PAIR std::pair<std::string, Location*>
#define LOCATION_MAP std::map<std::string, Location*, std::greater<std::string> >
#define LOCATION_ITT std::map<std::string, Location*, std::greater<std::string> >::iterator
#define TYPES_MAP std::map<std::string, std::string>
#define TYPES_ITT std::map<std::string, std::string>::iterator
#define TYPES_PAIR std::pair<std::string, std::string>
#define VECT_STR std::vector<std::string>
#define VECT_ERRORPIR std::vector<ERRPAGE_PAIR>
#define ITT_ERRORPIR std::vector<ERRPAGE_PAIR>::iterator
#define VECT_CLIENT std::vector<Client*>
#define ITT_CLIENT	std::vector<Client*>::iterator

#define TIME_OUT 60

class Server {

	public:
		Server();
		~Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);

	private:
		int									port; // done.
		size_t    							clientMaxBodySize; // done.
		bool								autoIndex; // done.
		std::string					host; // done.
		std::string					root; // done.
		std::string					serverName; // done.
		VECT_STR						index; // done.
		VECT_ERRORPIR				errorPages; // done.
		std::string 				defaultType; // done.
		TYPES_MAP						mimeType;
		LOCATION_MAP				locations; // done.

		int									_listen;
		VECT_CLIENT					clients;


		public:
			void setPort(int port);
		  void setClientMaxBodySize(size_t size);
			void setAutoIndex(bool autoIndex);
		  void setHost(const std::string& host);
		  void setRoot(const std::string& root);
		  void setServerName(const std::string& name);
		  void setIndex(const std::vector<std::string>& index);
		  void setErrorPages(const VECT_ERRORPIR& errorPages);
		  void setErrorPage(const	 ERRPAGE_PAIR errorPage);
		  void setDefaultType(const std::string& defaultType);
		  void setMimeType(const TYPES_MAP& mimeType);
		  bool setMimeType(const TYPES_PAIR& mimeType);
		  void setLocations(const LOCATION_MAP& locations);
		  void setSingleLocation(const LOCATION_PAIR& location);
		  void setItIndex(str_it begin, str_it end);

	public:
		std::vector<int>		CheckRepeatErrorPages;
		bool	listenRepeat;



		void final();

		bool	 start();
		bool	 handelFd(struct pollfd fd);
		void	 acceptClient();
		bool	 handelClient(ITT_CLIENT it, pollfd pfd);
		ITT_CLIENT	findClient(pollfd pfd);
		void	 checkTimeOut();

		void closeConnection(ITT_CLIENT it);



	public:
		int	getPort() const;
		size_t	getClientMaxBodySize() const;
		int	getListen() const;

		bool	getAutoIndex() const;

		std::string	getHost() const;
		std::string	getServerName() const;
		std::string getRoot() const;
		std::string getDefaultType() const;

		std::vector<std::string> getIndex() const;
		std::vector<ERRPAGE_PAIR> getErrorPages() const;
		LOCATION_MAP &getAllLocation();
		std::map<std::string, std::string> getMimeType() const;
		Location	&getLocation(std::string url);
};


std::ostream &operator<<(std::ostream &out, Server &server);
