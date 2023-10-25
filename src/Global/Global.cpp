
#include "../../include/Global.hpp"
#include <vector>


Global::Global(){}

Global::~Global(){}

Global::Global(const Global &copy){*this = copy;}

Global &Global::operator=(const Global &copy){
	servers = copy.servers;
	return *this;
}

void Global::addServer(Server *server){
	this->servers.push_back(server);
}

void  Global::print(){
		std::vector<Server *>::iterator it = servers.begin();

		for(;it != servers.end(); it++){
			std::cout << *(*it) << std::endl;
		}
}

void  Global::run()
{
	std::vector<Server *>::iterator it;

	it = servers.begin();

	for(;it != servers.end(); it++) {std::cout << (*it)->start() << std::endl;}
	int clientSocket;
	it = servers.begin();
	while (true)
	{
		std::cout << "polling" << std::endl;
		int ready = poll(this->servers[0]->getPollFds().data(), this->servers[0]->getPollFds().size(), -1);
		std::cout << "end polling" << std::endl;
		if (ready == -1) {
			perror("poll");
			break;
		}
		std::vector<pollfd> fds = servers[0]->getPollFds();

		for (unsigned int i = 0; i < fds.size(); i++) {

			if (fds[i].events & POLLIN) {

				if (fds[i].fd == this->servers[0]->getSocket()) {
					// Accept a new connection.
					clientSocket = accept(this->servers[0]->getSocket(), NULL, NULL);
					if (clientSocket != -1) {
						// Set the client socket to non-blocking mode.
						// fcntl(clientSocket, F_SETFL, O_NONBLOCK);

						this->servers[0]->SetFds((pollfd){clientSocket, (POLLIN | POLLOUT), 0});
					}
				} else
				{

					int BUFFER_SIZE = 1024;
					char buffer[BUFFER_SIZE + 1];

					std::cout << fds[i].fd << std::endl;
					int bytes = recv(fds[i].fd, buffer, BUFFER_SIZE, 0);
					if (bytes == -1) {
						perror("recv");
						break;
					}
					std::cout << buffer << std::endl;
					// close(fds[i].fd);
					// servers[0]->unsetFD(it);


				}
			}
			if (fds[i].revents & POLLOUT) {

				// Write a response to the client.
			}
		}
	}
}

