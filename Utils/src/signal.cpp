
#include <signal.h>
#include <iostream>

void sigHandler(int sig)
{
	std::cout << "server receive a Signal" << sig<<std::endl;
	signal(sig, sigHandler);
}

void sigChange()
{
	std::cout << "sigChange";
	for (size_t i = 0; i <= 22; i++)
	{
		if (i == SIGINT )
			continue;
		if  (signal(i, sigHandler) == SIG_ERR)
			std::cout << "error" << i<< std::endl;
	}
}
void sigRestDefault()
{
	for (size_t i = 0; i <= 22; i++)
		signal(i, sigHandler);
}

