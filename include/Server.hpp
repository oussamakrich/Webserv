#pragma  once

#include "includes.hpp"
#include <type_traits>
#include "Location.hpp"


class Server {

	private:	// Variables:
		short						port; // done.
		int							clientMaxBodySize; // done.
		std::string					host; // done.
		std::string					root; // done.
		std::string					serverName; // done.
		std::vector<std::string>	index; // done.
		std::vector<std::string>	errorPages; // done.
		std::string 				defaultType; // done.
		std::string 				accessLog; // done.
		std::string 				errorLog; // done.
		std::map<std::string, Location *>		locations; // done.
	public:		// Getters:
		short		getPort() const;
		std::string getErrorLog() const;
		std::string	getHost() const;
		std::string	getServerName() const;
		std::string getRoot() const;
		std::vector<std::string> getIndex() const;
		int			getClientMaxBodySize() const;
		std::vector<std::string> getErrorPages() const;
		std::string getDefaultType() const;
		std::string getAccessLog() const;

	public:		// Setters:
		void		setErrorLog(std::vector<TOKEN_PAIR>::iterator &i);
		void		setAccessLog(std::vector<TOKEN_PAIR>::iterator &i);
		void		setDefaultType(std::vector<TOKEN_PAIR>::iterator &i);
		void		setClientMaxBodySize(std::vector<TOKEN_PAIR>::iterator &i);
		void		setIndex(std::vector<TOKEN_PAIR>::iterator &i);
		void		setPort(std::vector<TOKEN_PAIR>::iterator &i);
		void		setHost(std::vector<TOKEN_PAIR>::iterator &i);
		void		setServerName(std::vector<TOKEN_PAIR>::iterator &i);
		void 		setRoot(std::vector<TOKEN_PAIR>::iterator &i);
		void		setErrorPages(std::vector<TOKEN_PAIR>::iterator &i);
		void		setLocation(std::vector<TOKEN_PAIR>::iterator &it);
	public:		// Canonical Form:
		~Server();
		Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);
		void print(){
			std::map<std::string, Location *>::iterator it;
			for(it=locations.begin();it != locations.end();it++){
				std::cout << "url : " <<it->first << std::endl;
				it->second->print(); 
		}
		};
	private:
};
