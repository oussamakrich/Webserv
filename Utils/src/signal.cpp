
#include <signal.h>
#include <iostream>

void sigHandler(int sig)
{
	errno = 0;
	std::cout << "server receive a Signal" << sig<<std::endl;
}

void sigChange()
{
	for (size_t i = 0; i <= 22; i++)
	{
		if (i == SIGINT || i == SIGABRT || i == SIGSEGV) 
			continue;
		signal(i, sigHandler);
	}
}

void sigRestDefault()
{
	for (size_t i = 0; i <= 22; i++)
		signal(i, sigHandler);
}

