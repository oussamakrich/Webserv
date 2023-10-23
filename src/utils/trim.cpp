
#include "../../include/includes.hpp"

void Otrim(std::string &line){

	  size_t startPos, endPos;

		startPos = line.find_first_not_of(" \t\r");
    if (startPos != std::string::npos)
        line = line.substr(startPos);

    endPos = line.find_last_not_of(" \t\r");
    if (endPos != std::string::npos)
        line = line.substr(0, endPos + 1);
}
