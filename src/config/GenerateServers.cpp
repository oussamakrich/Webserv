
#include "../../include/GenerateServers.hpp"
#include <vector>

// Start: Canonical Form
GenerateServers::~GenerateServers() {}
GenerateServers::GenerateServers() {}
GenerateServers::GenerateServers(const GenerateServers &copy){
	*this = copy;
}
GenerateServers &GenerateServers::operator=(const GenerateServers &copy){
	if (this != &copy)
	{
	}
	return (*this);
}
// End: Canonical Form





int GenerateServers::getValueType(std::vector<TOKEN_PAIR>::iterator &i)
{
	while (i->second != BRACKETCLOSE)
	{
		if (i->second == VALUE)
		{
			if (trim(i->first) == "host") return (i++, HOST);
			else if (trim(i->first) == "port") return (i++, PORT);
			else if (trim(i->first) == "server_name") return (i++, SERVER_NAME);
			else if (trim(i->first) == "error_page") return (i++, ERROR_PAGE);
			else if (trim(i->first) == "max_body_size") return (i++, CLIENT_MAX_BODY_SIZE);
			else if (trim(i->first) == "index") return (i++, INDEX);
			else if (trim(i->first) == "root") return (i++, ROOT);
			else if (trim(i->first) == "default_type") return (i++, DEFAULT_TYPE);
			else if (trim(i->first) == "error_log") return (i++, ERROR_LOG);
			else if (trim(i->first) == "access_log") return (i++, ACCESS_LOG);
			// else if (trim(i->first) == "location") return (i++, LOCATION);
			// else if (trim(i->first) == "allow_methods") return (i++, ALLOW_METHODS);
			// else if (trim(i->first) == "include") return (i++, INCLUDE);
			// else if (trim(i->first) == "server") return (i++, SERVER);
			// else if (trim(i->first) == "}") return (i++, BRACKETCLOSE);
			// else if (trim(i->first) == "{") return (i++, BRACKETOPEN);
			// else if (trim(i->first) == ";") return (i++, SEMICOLONE);
			// else if (trim(i->first) == ":") return (i++, COLONE);
			// else if (trim(i->first) == "esp") return (i++, ESP);

		}
		i++;
	}
	return (-1);
}

void GenerateServers::Generator(std::vector<TOKEN_PAIR> &tokens, std::vector<Server*> &servers)
{

	for (std::vector<TOKEN_PAIR>::iterator i = tokens.begin(); i != tokens.end(); i++)
	{
		if (i->second == SERVER)
		{
			Server *server  = new Server();
			while (i->second != BRACKETCLOSE)
			{
				switcher = getValueType(i);
				switch (switcher)
				{
					case HOST:
						server->setHost(i);
						break;
					case PORT:
						server->setPort(i);
						break;
					case SERVER_NAME:
						server->setServerName(i);
						break;
					case ERROR_PAGE:
						server->setErrorPages(i);
						break;
					case CLIENT_MAX_BODY_SIZE:
						server->setClientMaxBodySize(i);
						break;
					case INDEX:
						server->setIndex(i);
						break;
					case ROOT:
						server->setRoot(i);
						break;
					case DEFAULT_TYPE:
						server->setDefaultType(i);
						break;
					case ERROR_LOG:
						server->setErrorLog(i);
						break;
					case ACCESS_LOG:
						server->setAccessLog(i);
						break;
				}
				i++;
			}
			servers.push_back(server);
		}
	}
	int number = 1;
	for (std::vector<Server*>::iterator i = servers.begin(); i != servers.end(); i++)
	{
		std::cout << "Server: " << number << std::endl;
		std::cout << "Host: " << (*i)->getHost() << std::endl;
		std::cout << "Port: " << (*i)->getPort() << std::endl;
		std::cout << "Server Name: " << (*i)->getServerName() << std::endl;
		std::cout << "Error Pages: ";
		std::vector<std::string> errorPages = (*i)->getErrorPages();
		for (std::vector<std::string>::iterator j = errorPages.begin(); j != errorPages.end(); j++)
		{
			std::cout << *j << " ";
		}
		std::cout << std::endl;
		std::cout << "Client Max Body Size: " << (*i)->getClientMaxBodySize() << std::endl;
		std::cout << "Index: ";
		std::vector<std::string> index = (*i)->getIndex();
		for (std::vector<std::string>::iterator j = index.begin(); j != index.end(); j++)
		{
			std::cout << *j << " ";
		}
		std::cout << std::endl;
		std::cout << "Root: " << (*i)->getRoot() << std::endl;
		std::cout << "Default Type: " << (*i)->getDefaultType() << std::endl;
		std::cout << "Error Log: " << (*i)->getErrorLog() << std::endl;
		std::cout << "Access Log: " << (*i)->getAccessLog() << std::endl;
		std::cout << std::endl;
		number++;
	}

	// std::cout << servers.size() << std::endl;
	// for (std::vector<TOKEN_PAIR>::iterator i = tokens.begin(); i != tokens.end(); i++)
	// {
	// 	std::cout << "|" << i->first << "| |" << i->second << "|" << std::endl;
	// }
	(void )servers;
}
