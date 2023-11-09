
#include <signal.h>
#include <iostream>

void sigHandler(int sig)
{
	std::cout << "server receive a Signal" << sig<<std::endl;
	signal(sig, sigHandler);
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

