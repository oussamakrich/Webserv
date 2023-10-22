#include <iostream>
int main(int ac, char *av[]) {



	std::string str = "\" hello world       hello world";

	if (str.find('\"', 1) == std::string::npos)
		return 1;
	std::cout << str.substr(1, str.find('\"', 1) - 1);
	return 0;
}
