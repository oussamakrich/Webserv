
#include <signal.h>
#include <iostream>
#include "../../include/includes.hpp"

void sigHandler(int sig)
{
	errno = 0;
	signal(sig, sigHandler);
	std::cout << "Websever receive a Single ->" << sig << std::endl;
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

