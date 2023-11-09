#include "../../include//includes.hpp"
#include <sys/_types/_size_t.h>

std::string ltrim(const std::string &s) {
    std::string result = s;
    size_t pos = result.find_first_not_of(" \t\n\r");
    if (pos != std::string::npos) {
        result = result.substr(pos);
    }
    return result;
}

std::string rtrim(const std::string &s) {
    std::string result = s;
    size_t pos = result.find_last_not_of(" \t\n\r");
    if (pos != std::string::npos) {
        result = result.substr(0, pos + 1);
    }
    return result;
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}


std::vector<std::string> splitStream(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}
<<<<<<< HEAD
=======

void removeSlash(std::string &path) {
    for(size_t i =0 ; i < path.size(); i++)
        if (path[i] == '/')
        {
            i++;
            while (path[i] && path[i] == '/')
                path.erase(i, 1);
        }
}
>>>>>>> origin/mergeGetPost
