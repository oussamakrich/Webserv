#include "../../include/includes.hpp"
#include <sys/time.h>

std::string hexToChar(std::string hex){
    int i;
    std::string c;
    std::stringstream ss(hex);

    if (hex.size() != 2 || hex.find_first_not_of("1234567890abcdefABCDEF") != hex.npos)
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
    if (ss.str().size() != 2)
        return  "0" + ss.str();
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

char *Responsejoin(const char *s1, char *s2, size_t size1, size_t size2)
{
	char		*tmp;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	tmp = new char[size1 + size2 + 1];
	while (i < size1)
	{
		tmp[i] = s1[i];
		i++;
	}
	while (j < size2)
	{
		tmp[i + j] = s2[j];
		j++;
	}
	if (size2 > 0)
		delete[] s2;
	return (tmp);
}

std::string generateId(){

	timeval tm;
    	gettimeofday(&tm, nullptr);

	std::stringstream ss;
	ss <<tm.tv_sec << "@" << tm.tv_usec;
	return ss.str();
}
