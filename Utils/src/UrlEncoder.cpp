#include "../../include/includes.hpp"
#include <ios>

std::string hexToChar(std::string hex){
    int i;
    std::string c;
    std::stringstream ss(hex);

    if (hex.size() != 2)
        return'%' + hex;

    ss >> std::hex >> i;
    c = static_cast<char>(i);

    return c;
}

std::string UrlDecode(std::string codedStr){
    std::string uncodedStr; 

    for(unsigned int i=0;i< codedStr.size(); i++){
        if(codedStr[i] == '%'){
           uncodedStr += hexToChar(codedStr.substr(i + 1, 2));
           i += 2;
        }
        else
            uncodedStr += codedStr[i];
    }
    return uncodedStr;
}

std::string CharToHex(char c){

    int i = static_cast<int>(c);
    std::stringstream ss;

    ss << std::hex  << i;
    return ss.str();
}

std::string UrlEncode(std::string simpleStr)
{
    std::string codedStr;
    std::string unreserved = "-_.~";
    for (unsigned int i=0;i< simpleStr.size(); i++){
        if (isalnum(simpleStr[i]) || unreserved.find(simpleStr[i]) != unreserved.npos) 
            codedStr += simpleStr[i];
        else 
            codedStr += "%" + CharToHex(simpleStr[i]);
    }

    return codedStr;
}

