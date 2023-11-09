# ifndef __LOGGER_H__
# define __LOGGER_H__
# include <string>
# include <iostream>
# include "../../HttpElement/include/Server.hpp"
#include <fstream>

class Logger
{


private :
    std::fstream _log;
    bool logFormat(const std::string &logtype, std::string &msg);
public :
    enum e_logtype {INFO, WARNING , DEBUG, ERROR};
    Logger(const std::string logfile);
    ~Logger();
    bool Info(std::string msg);
    bool Warning(std::string  msg);
    bool Debug(std::string msg);
    bool Error(std::string  msg);

    bool isGood(); // return status of open log file;
    static bool fastLog(int type, std::string file, std::string msg);
    static bool fastLog(int type, std::string file, std::string msg, Server &serv);

};

#endif
