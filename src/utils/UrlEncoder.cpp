#include "../../include/includes.hpp"
#include <sstream>
#include <string>

char hexToChar(std::string hex){
    char c;
    int i;
    std::stringstream ss(hex);
    ss >> std::hex >> i;

    c = static_cast<char>(i);
    return c;
}

std::string UrlDecode(std::string codedStr){
    std::string uncodedStr; 

    for(unsigned int i=0;i< codedStr.size(); i++){
        if(codedStr[i] == '%'){
           uncodedStr += hexToChar(codedStr.substr(i, 2));
           i += 2;
        }
        else
            uncodedStr += codedStr[i];
    }

    return uncodedStr;
}

