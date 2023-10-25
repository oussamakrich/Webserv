
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
// void  Global::run()
// {
// 	Server serv = *serv.
// 	while (true)
// 	{
// 		int ready;
// 			ready = poll(serv.getPollFds().data(),serv.getPollFds().size(),-1);
// 			std::cout << "ready number"<< ready << endl;
// 		vector<pollfd> &vec = get
// (int i)
// 		{
// 			/* code */
// 		}
		
// 	}
	

// }

void  Global::run()
{
	Server serv = *servers[0]
	serv.start();
	int clientSocket;
	std::vector<pollfd> &fds = serv.getPollFds();
	int number = 0;
	while (true)
	{

		std::cerr << "wait for new fd " << std::endl;
		int ready = poll(serv.getPollFds().data(), serv.getPollFds().size(), -1);
		
		if (ready == -1) {
			perror("poll");
			break;
		}
		for (unsigned int i = 0; i < fds.size(); i++) {

			if (fds[i].events & POLLIN) {

				if (fds[i].fd == serv.getSocket()) {
					// Accept a new connection.
					clientSocket = accept(serv.getSocket(), NULL, NULL);
					std::cerr << "----> accept connection number "<< number << " >--"<< std::endl;
					std::cerr<< "accept fd " <<endl;
					number++;
					if (clientSocket != -1) {
						// Set the client socket to non-blocking mode.
						//fcntl(clientSocket, F_SETFL, O_NONBLOCK);
						serv.SetFds((pollfd){clientSocket, (POLLIN | POLLOUT), 0});
					}
				}
				 else
				{
					int BUFFER_SIZE = 1024;
					char buffer[BUFFER_SIZE + 1];
					int bytes = recv(fds[i].fd, buffer, BUFFER_SIZE, 0);
					if (bytes == -1) 
						perror("cant ', read \n");
					else	
					std::cout << buffer << std::endl;
					close(fds[i].fd);
					std::cerr<< "close fd " << fds[i].fd << endl;
					 fds.erase(fds.begin() + i);
				}
			}
		}
	}
}

