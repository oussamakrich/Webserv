#include "../../include/Server.hpp"
#include "../../include/includes.hpp"
#include <sstream>
#include <string>
#include <vector>

short Server::port = 0;
// Start: Canonical Form
Server::Server()
{
}

Server::~Server()
{
}

Server::Server(const Server &copy)
{
	*this = copy;
}

Server &Server::operator=(const Server &copy)
{
	if (this != &copy)
	{
		this->port = copy.port;
		this->host = copy.host;
		this->serverName = copy.serverName;
	}
	return (*this);
}

// End: Canonical Form


// Start: Getters

std::vector<std::string> Server::getErrorPages() const
{
	return (this->errorPages);
}

std::string Server::getRoot() const
{
	return (this->root);
}

short Server::getPort() const
{
	return (this->port);
}

std::string Server::getDefaultType() const
{
	return (this->defaultType);
}


std::string Server::getHost() const
{
	return (this->host);
}

std::string Server::getServerName() const
{
	return (this->serverName);
}

std::vector<std::string> Server::getIndex() const
{
	return (this->index);
}

int Server::getClientMaxBodySize() const
{
	return (this->clientMaxBodySize);
}

std::string Server::getAccessLog() const
{
	return (this->accessLog);
}

std::string Server::getErrorLog() const
{
	return (this->errorLog);
}

// End: Getters


// Start: Setters

void Server::setErrorPages(std::vector<TOKEN_PAIR>::iterator &i)
{

	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
			this->errorPages.push_back(trim(i->first));
		i++;
	}

	std::string last = trim(this->errorPages.back());
	this->errorPages.pop_back();
	for (std::vector<std::string>::iterator it = this->errorPages.begin(); it != this->errorPages.end(); it++)
	{
		std::string	tmp = last;
		tmp.replace(tmp.find("@"), 1, *it);
		it->clear();
		*it = tmp;

	}
}


void Server::setErrorLog(std::vector<TOKEN_PAIR>::iterator &i)
{
	int found;

	found = 0;
	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
		{
			this->errorLog = trim(i->first);
			found++;
		}
		i++;
	}
	if (found != 1)
		throw std::runtime_error("Error log not found or more than one error log found.");
}

void	Server::setDefaultType(std::vector<TOKEN_PAIR>::iterator &i)
{
	int found;

	found = 0;
	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
		{
			this->defaultType = trim(i->first);
			found++;
		}
		i++;
	}
	if (found != 1)
		throw std::runtime_error("Default type not found or more than one default type found.");
}

void Server::setClientMaxBodySize(std::vector<TOKEN_PAIR>::iterator &i)
{
	int found;

	found = 0;
	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
		{

			std::stringstream ss(trim(i->first).substr(0, trim(i->first).size() - 2));
			if (!(ss >> this->clientMaxBodySize))
				throw std::runtime_error("Client max body size: not a valid number.");
			found++;
		}
		i++;
	}
	if (found != 1)
		throw std::runtime_error("Client max body size not found or more than one client max body size found.");
}

void Server::setIndex(std::vector<TOKEN_PAIR>::iterator &i)
{
	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
			this->index.push_back(trim(i->first));
		i++;
	}
	if (this->index.size() == 0)
		throw std::runtime_error("Index not found.");
}

void Server::setRoot(std::vector<TOKEN_PAIR>::iterator &i)
{
	int found;

	found = 0;
	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
		{
			this->root = trim(i->first);
			found++;
		}
		i++;
	}
	if (found != 1)
		throw std::runtime_error("Root not found or more than one root found.");
}

void Server::setPort(std::vector<TOKEN_PAIR>::iterator &i)
{
	int found;

	found = 0;
	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
		{
			std::stringstream ss(trim(i->first));
			if (!(ss >> this->port))
			{

				throw std::runtime_error("Port: " + i->first + "not a valid number.");
			}
			found++;
		}
		i++;
	}
	if (found != 1)
		throw std::runtime_error("Port not found or more than one port found.");
}

void Server::setHost(std::vector<TOKEN_PAIR>::iterator &i)
{
	int	found;

	found = 0;
	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
		{
			this->host = trim(i->first);
			found++;
		}
		i++;
	}
	if (found != 1)
		throw std::runtime_error("Host not found or more than one host found.");
}

void Server::setServerName(std::vector<TOKEN_PAIR>::iterator &i)
{
	int found;

	found = 0;
	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
		{
			this->serverName = trim(i->first);
			found++;
		}
		i++;
	}
	if (found != 1)
		throw std::runtime_error("Server name not found or more than one server name found.");
}

void Server::setAccessLog(std::vector<TOKEN_PAIR>::iterator &i)
{
	int found;

	found = 0;
	while (i->second != SEMICOLONE)
	{
		if (i->second == VALUE)
		{
			this->accessLog = trim(i->first);
			found++;
		}
		i++;
	}
	if (found != 1)
		throw std::runtime_error("Access log not found or more than one access log found.");
}

// End: Setters
