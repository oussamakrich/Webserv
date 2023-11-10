
#include "../include/Server.hpp"
#include "../include/Global.hpp"
#include "../../Response/include/GenerateResponse.hpp"
#include "../../Uploader/include/Upload.hpp"
#include <sys/time.h>

std::string timeString(double tm)
{
	stringstream ss;
	if (tm  > 1000000)
		ss << tm / 1000000.0 << " s";
	else if (tm  > 1000)
		ss << tm / 1000 << " ms";
	else
		ss << tm  << " us";
return ss.str();
}
static double GetTM()
{
	timeval tm;
	double val;
	gettimeofday(&tm, NULL);
	val = tm.tv_sec * 1000000 + tm.tv_usec;
	return val;
}

bool creatSocket(int *listen, addrinfo *MyAddr){
	*listen = socket(MyAddr->ai_family , MyAddr->ai_socktype , 0);
	if (*listen == -1){
		std::cerr << "Socket : failed " << std::endl;
		freeaddrinfo(MyAddr);
		return false;
	}
	int opt = 1;
	if (setsockopt(*listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		return false;
	}
	return true;
}

bool Server::start(){
	if (listenRepeat == true) // NOTE : if the server is repeated dont start it
		return true;

	stringstream PortString;
	PortString << this->port;
	struct addrinfo hints;
	bzero(&hints, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *MyAddr;
	int	ret = getaddrinfo(this->getHost().c_str(), PortString.str().c_str(), &hints, &MyAddr);
	if(ret){
		gai_strerror(ret);
		freeaddrinfo(MyAddr);
		return false;
	}
	if (!creatSocket(&_listen, MyAddr))
		return false;

	if (bind(_listen, MyAddr->ai_addr,MyAddr->ai_addrlen) != 0){
		std::cerr << "bind : failed " << std::endl;
		freeaddrinfo(MyAddr);
		return false;
	}
	freeaddrinfo(MyAddr);
	if (listen(_listen, SOMAXCONN) == -1){
		std::cerr << "listen : failed " << std::endl;
		return false;
	}
	fcntl(_listen, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	Global::insertFd(_listen);
	return true;
}

void Server::acceptClient(){
	Client	*newClient = NULL;
	struct sockaddr sockaddr;
	bzero(&sockaddr, sizeof(sockaddr));
	socklen_t len = sizeof(sockaddr);
	int clientFd = accept(_listen, &sockaddr, &len);
	if (clientFd  == -1){
		std::cerr << "ERROR : Connection failed" << std::endl;
		return;
	}
	newClient = new Client(this->clientMaxBodySize, clientFd);
	newClient->setAddr(sockaddr);
	fcntl(clientFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	newClient->TM = GetTM();
	this->clients.push_back(newClient);
	Global::insertFd(clientFd);
	std::cout << serverName + "new Client fd = " << clientFd << std::endl;
	

}
ITT_CLIENT Server::findClient(pollfd pfd){
	ITT_CLIENT it = clients.begin();

	for(; it != clients.end(); it++)
		if (pfd.fd == (*it)->getFd())
				break ;
	return it;
}

void Server::closeConnection(ITT_CLIENT it){
	Client *client = *it;
	std::cout << std::boolalpha;
	std::cout << "--------------------------------------------\n";
	std::cout << "connection closed:" << client->getFd() << " time " <<  timeString(GetTM() - client->TM ) <<" us"<< std::endl;
	std::cout <<"CGI STATUS : " << client->CGIFinish << std::endl;
	std::cout << "" <<( client->pfd.events  ==  POLLOUT ? "POLLOUT" :  "POOLIN ?");
	std::cout<<  "  Keep ALive :" << client->keepAlive;
	clients.erase(it);
	Global::removeFd(client->getFd());
	delete client;
}

bool Server::handelClient(ITT_CLIENT it){
	if (it == clients.end())	return false;

	Client *client = *it;
	client->setLastTime(time(NULL));
	if (client->IhaveUpload){
		Upload reminder(*this, *client->response);
		client->IhaveUpload = client->response->iHaveUpload;
	}
	else if (client->IhaveCGI && !client->CGIFinish)
		client->CgiRequest(it, *this);
	else if (client->IhaveResponse)
		client->OldRequest(it, *this);
	else if (!client->NewRequest(it, *this) && !client->response->errorInSend){
		client->response->sendErrorResponse(*this, client->getFd());
		closeConnection(it);
		return true;
	}
	if (client->response && client->response->errorInSend){
		closeConnection(it);
		return true;
	}
	if (!client->keepAlive && !client->IhaveResponse)
		closeConnection(it);
	return true;
}

bool Server::handelFd(struct pollfd pfd){

	if (pfd.fd == _listen){
		if (pfd.revents & POLLOUT)
			return false;
		this->acceptClient();
		return true;
	}
	else
		 return this->handelClient(findClient(pfd));
}

void Server::checkTimeOut(){
	ITT_CLIENT it = clients.begin();
	Client *client;
		std::cout << "\t\t\t@#########################< STATRT >################################>" << std::endl;
	while (it != clients.end()){
		client = *it;
		std::time_t tm = client->getLastTime();
		std::time_t now = std::time(NULL);
		if(now - tm >= 1){
	 		std::cout << "Client fd: " << client->getFd()  << "TIME : " << now - tm<< "\n";
			closeConnection(it);
			continue;
		}
		it++;
	}
			std::cout << "\t\t\t@#########################< END  >################################>" << std::endl;
}
