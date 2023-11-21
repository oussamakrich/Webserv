
#include "../include/Server.hpp"
#include "../include/Global.hpp"
#include "../../Response/include/GenerateResponse.hpp"
#include "../../Uploader/include/Upload.hpp"
#include "../../Utils/include/Logger.hpp"

void Server::acceptClient(){

	Client	*newClient = NULL;
	int clientFd = accept(_listen, NULL, NULL);
	if (clientFd  == -1){
		perror("accept");
		return;
	}
	fcntl(clientFd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	newClient = new(std::nothrow) Client(this->clientMaxBodySize, clientFd);
	if (newClient == NULL){
		std::cerr << "ERROR : new Client failed" << std::endl;
		return;
	}
	std::cout << "new Client Accepted" << std::endl;
	this->clients.push_back(newClient);
	Global::insertFd(clientFd);
}

ITT_CLIENT Server::findClient(pollfd pfd){

	ITT_CLIENT it = clients.begin();
	for(; it != clients.end(); it++)
		if (pfd.fd == (*it)->getFd())
				break ;
	return it;
}

bool Server::handelFd(struct pollfd pfd){

	if (pfd.fd == _listen){
		if (pfd.revents & POLLOUT)
			return false;
		this->acceptClient();
		return true;
	}
	else
		 return this->handelClient(findClient(pfd), pfd);
}

void Server::closeConnection(ITT_CLIENT it){

	std::cout << "Connection Closed" << std::endl;
	Client *client = *it;
	clients.erase(it);
	Global::removeFd(client->getFd());
	delete client;
}

bool Server::handelClient(ITT_CLIENT it, pollfd pfd){
	if (it == clients.end())	return false;

	Client *client = *it;
	if (pfd.revents & POLLHUP){
		client->clearClient();
		this->closeConnection(it);
		return true;
	}
	client->setLastTime(time(NULL));
	if (client->IhaveUpload)
		client->ClientUpload(*this);
	else if (client->IhaveCGI && !client->CGIFinish)
		client->CgiRequest();
	else if (client->IhaveResponse){
		if (client->OldRequest() == false)
			closeConnection(it);
	}
	else if (!client->NewRequest(*this)){
		client->response->sendErrorResponse(client->getFd(), client->keepAlive);
		client->resetClient();
		client->switchEvent(pfd.fd, POLLIN);
		return true;
	}
	client->setLastTime(time(NULL));
	if (!client->keepAlive && !client->IhaveResponse)
		closeConnection(it);
	return true;
}

void Server::checkTimeOut()
{	
	waitpid(-1, 0, WNOHANG);
	ITT_CLIENT it = clients.begin();
	Client *client;

	while (it != clients.end()){
		client = *it;
		std::time_t tm = client->getLastTime();
		std::time_t now = std::time(NULL);
		if(now - tm >= TIME_OUT){
	 		std::cout << "Client Time-Out" << std::endl;

			closeConnection(it);
			continue;
		}
		it++;
	}
}

