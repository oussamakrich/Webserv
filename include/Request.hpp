#include "Server.hpp"
#include "includes.hpp"


class Request{
	private:
		std::string							method;
		std::string							path;
		std::string							version;
		std::map<std::string, std::string>	headers;
		std::string							body;
		int									content_length;

	public: // Getters:
		std::string const &getMethod() const;
		std::string const &getPath() const;
		std::string const &getVersion() const;
		std::string const &getHeader(std::string const &key) const;
		std::string const &getBody() const;
	public: // setters:
		void setMethod(std::string const &method);
		void setPath(std::string const &path);
		void setVersion(std::string const &version);
		void setHeaders(std::pair<std::string, std::string> &headers);
		void setBody(std::string const &body);

	public: // Canonical Form:
		Request();
		Request(const Request &copy);
		Request &operator=(const Request &copy);
		~Request();
	public: // request methods.
		void CheckRequestFormat(Server const &server);
		void RequestHandler(std::string const &request, Server const &server);
		Location const &get_Matched_Location_For_Request_Uri(Server const &server);
};
