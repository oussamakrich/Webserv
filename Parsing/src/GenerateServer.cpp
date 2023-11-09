#include "../include/GenerateServer.hpp"
#include "../include/GenerateLocation.hpp"

void error(std::string error){
	std::cerr << RED"Syntax Error : "<< RESET << error  << std::endl;
	exit(1);
}

Server *GenerateServer::NewServer(TOKEN_IT &it){

	Server *server = new Server();
	while(it->first != CLOSE_C_BRACKET){
		switch (it->first) {
			case LOCATION				: fillLocation(*server, ++it);		break;
			case TYPES					:	SetTypes(*server, ++it);				break;
			case MAX_BODY_SIZE	: SetInt(*server, it);						break;
			case ERROR_PAGES		: SetErrorPages(*server, ++it);	break;
			case INDEX					: SetMultiValue(*server, it);		break;
			case ROOT						: SetSingleValue(*server, it);		break;
			case LISTEN					: SetHostAndPort(*server, ++it); break;
			case SERVER_NAME		: SetSingleValue(*server,it);		break;
			case DEFAULT_TYPE		: SetSingleValue(*server, it);		break;
			case ERROR_LOG			: SetSingleValue(*server, it);		break;
			case ACCESS_LOG			: SetSingleValue(*server, it);		break;
			case AUTO_INDEX			: SetSingleValue(*server, it);		break;
			case OPEN_C_BRACKET :	break;
			case COLON					:	break;
			case SEMICOLON			:	error("Unexpected SEMICOLONE");
			default							: error("Unexpected key : " + it->second);
		}
		it++;
	}
	if (server->getRoot().empty())
		error("root is required");
	server->final();
	return server;
}

int GenerateServer::parseErrorPage(std::vector<int> &CheckRepeat,std::string codeValue){
	if (codeValue.find_first_not_of("0123456789") != codeValue.npos)
		error("error code should be a number in ErrorPage");
	if (codeValue.size() != 3)
		error("error code range between 400 To 599");
	int errorCode = atoi(codeValue.c_str());
	if (errorCode < 400 || errorCode > 599)
		error("error code range between 400 To 599");
	if (std::find(CheckRepeat.begin(), CheckRepeat.end(), errorCode) != CheckRepeat.end())
			error("errorCode is repeated twice in ErrorPage");
	CheckRepeat.push_back(errorCode);
	return errorCode;
}

void GenerateServer::SetErrorPages(Server &ser, TOKEN_IT &it){
	ERRPAGE_PAIR pair;
	int	code;
	std::string value;
	VECT_ERRORPIR errorPages = ser.getErrorPages();


	if (it->first != WORD)	error("Expect NUMBER after ErrorPage");
	code = parseErrorPage(ser.CheckRepeatErrorPages, it->second);
	it++;
	if (it->first != WORD)	error("Expect Tow value after ErrorPage");
	value = it->second;
	it++;
	if (it->first != SEMICOLON)	error("Expect only Tow value after ErrorPage");

	pair.first = code;
	pair.second = value;
	if (std::find(errorPages.begin(), errorPages.end(), pair) != errorPages.end())
			error("value is repeated twice in ErrorPage");
	ser.setErrorPage(pair);
}

void GenerateServer::fillLocation(Server &ser, TOKEN_IT &it){

	Location *location =	GenerateLocation::generateLocation(it);
	std::string uri = location->getUri();
	ser.setSingleLocation(std::make_pair(uri, location));
}

void GenerateServer::SetTypes(Server &ser, TOKEN_IT &it){
	std::string value, key;

	if (it->first == OPEN_C_BRACKET)
		it++;
	if (it->first != WORD)	error("Types should be followd by WORD");

	while (it->first != CLOSE_C_BRACKET){
		value = it->second;
		it++;
		if (it->first != WORD)	error("Types Expect Key and Value");
		while (it->first != SEMICOLON && it->first != CLOSE_C_BRACKET){
			key = it->second;
			if (!ser.setMimeType(std::make_pair(key, value)))
				error("duplicate Type");
			it++;
		}
		if (it->first == CLOSE_C_BRACKET){
			it++;
			break;
		}
		it++;
	}
}
void checkReapeat(Server &ser, Token Key){
	if (Key == ROOT && !ser.getRoot().empty())
		error("\"root\" directive is duplicate");
	if (Key == DEFAULT_TYPE && !ser.getDefaultType().empty())
		error("\"default_type\" directive is duplicate");
}

bool parseIndex(std::string value){
	if (value == "on")		return true;
	if (value == "off")		return false;
	error("autoindex should be on or off");
	return false;
}

void GenerateServer::SetSingleValue(Server &ser,TOKEN_IT &it){

	std::string value, keyWord;
	Token				key = it->first;

	keyWord = it->second;
	it++;
	if (it->first != WORD)
		error("Error: " + keyWord + " Should be followed by WORD");
	value = it->second;
	it++;
	if (it->first != SEMICOLON)
		error("Error: " + keyWord +" Accept only one Value");
	checkReapeat(ser, key);
	if (key == SERVER_NAME)				ser.setServerName(value);
	else if (key == AUTO_INDEX)		ser.setAutoIndex(parseIndex(value));
	else if (key == DEFAULT_TYPE)	ser.setDefaultType(value);
	else if (key == ACCESS_LOG)		ser.setAccessLog(value);
	else if (key == ERROR_LOG)		ser.setErrorLog(value);
	else if (key == ROOT)					ser.setRoot(value);
}

void GenerateServer::SetMultiValue(Server &ser, TOKEN_IT &it){

	std::vector<std::string> index = ser.getIndex();
	std::vector<std::string> tmpIndex;
	while (it->first != SEMICOLON)
	{
		if (it->first == WORD){
			if (std::find(index.begin(), index.end(), it->second) != index.end())
				error(it->second + " is repeated twice in index");
			tmpIndex.push_back(it->second);
		}
		it++;
	}
	ser.setItIndex(tmpIndex.begin(), tmpIndex.end());
}

void GenerateServer::SetInt(Server &ser, TOKEN_IT &it){
	int		size;
	while (it->first != SEMICOLON)
	{
		if (it->first == WORD)
			size = atoi(it->second.c_str());
		it++;
	}
	ser.setClientMaxBodySize(size);
}

void	GenerateServer::handelOne(Server &ser,std::string line){
	size_t pos= line.find_first_not_of("0123456789");
	if (pos == line.npos){
		if (line.size() > 5)
			error("Port is grather tha max");
			ser.setPort(atoi(line.c_str()));
	}
	else
		ser.setHost(line);
}

void GenerateServer::hostV6(Server &ser, std::string line)
{
	std::string portStr;
	size_t pos = line.find(']');

	if (pos == line.npos)
			error("Listen '[' Should be closed");
	ser.setHost(line.substr(1, pos - 1));
	if (ser.getHost().find_first_not_of("ABCDEFabcdef0123456789:[]") != ser.getHost().npos)
		error("Unexpected Charachter in host");
	line.erase(0, pos + 1);
	if (!line.empty()){
		if (line.front() != ':' || line.back() == ':')
			error("Listen expect ':' after host");
		portStr = line.substr(1);
		if (portStr.find_first_not_of("0123456789") != line.npos || portStr.size() > 5)
			error("In Port Value");
		ser.setPort(atoi(portStr.c_str()));
	}
}

void GenerateServer::parseListen(Server &ser, std::string line){
	size_t pos;
	std::string portStr;
	if (line[0] == '[')
		hostV6(ser, line);
	else
	{
		pos = line.find(':');
		if (pos == line.npos)
			handelOne(ser, line);
		else
		{
			if (pos !=line.rfind(':'))
				error("Identifier repeated twice ':' ");
			ser.setHost(line.substr(0, pos));
			portStr = line.substr(pos + 1);
			if (portStr.find_first_not_of("0123456789") != line.npos || portStr.size() > 5)
				error("In Port Value");
			ser.setPort(atoi(portStr.c_str()));
		}
	}
	if (ser.getPort() <= 0 || ser.getPort() >  USHRT_MAX)
		error("Port out of Range");
}

void	GenerateServer::SetHostAndPort(Server &ser, TOKEN_IT &it){

	std::string value = it->second;
	it++;
	if (it->first != SEMICOLON)
		error("Listen accept one value");
	if(value.back() == ':')
		error("':' cannot be in the back");
	if(value.front() == ':')
		error("':' cannot be in the front");
	parseListen(ser, value);
}

