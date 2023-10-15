#include <iostream>
#include <string>
#include <algorithm>

int main(int ac, char *av[]) {


	std::string str = "/test/@.html";
	std::string err = "505";

	str.replace(str.find("@"), 1, err);
	std::cout << str << std::endl;
	return 0;
}
