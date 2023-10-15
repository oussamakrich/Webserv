#include "../../include/includes.hpp"

std::string ltrim(const std::string &s) {
    std::string result = s;
    size_t pos = result.find_first_not_of(" \"\t\n\r");
    if (pos != std::string::npos) {
        result = result.substr(pos);
    }
    return result;
}

// Trim spaces from the end of a string
std::string rtrim(const std::string &s) {
    std::string result = s;
    size_t pos = result.find_last_not_of(" \"\t\n\r");
    if (pos != std::string::npos) {
        result = result.substr(0, pos + 1);
    }
    return result;
}

// Trim spaces from both the beginning and end of a string
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}
