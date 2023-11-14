#include  "../include/Logger.hpp"
#include "../../HttpElement/include/Global.hpp"

Logger::Logger(const std::string logfile): _log(logfile, std::ios::app){}

Logger::~Logger(){_log.close();}

bool Logger::logFormat(const std::string &logtype, std::string &msg)
{
    if (_log.good() == false) return false;
	_log << timeToString(getTime() - Global::time) << "    ";
    _log << logtype;
	_log << " : ";
	_log << msg;
	_log << "\n";
    return true;

}

bool Logger::Info(std::string msg)
{
	std::string tmp = "INFO";
    return logFormat(tmp, msg);
}

bool Logger::Warning(std::string msg)
{
	std::string tmp = "WARNING";
    return logFormat(tmp, msg);
}

bool Logger::Debug(std::string msg)
{
	std::string tmp = "DEBUG";
    return logFormat(tmp, msg);
}

bool Logger::Error(std::string  msg)
{
	std::string tmp = "ERROR";
    return logFormat(tmp, msg);
}

bool Logger::isGood()
{
	return _log.good();
} // return status of open log file;
bool Logger::fastLog(int type, std::string file, std::string msg)
{
	Logger lg(file);
	bool res ;
	switch (type)
	{
		case INFO		:	res = lg.Info(msg);			break;
		case DEBUG		:	res = lg.Debug(msg);		break;
		case WARNING		:	res = lg.Warning(msg);		break;
		case ERROR 		: 	res = lg.Error(msg);		break;
	}
	return res;
}

bool Logger::fastLog(int type,std::string file,std::string msg, Server &serv)
{
	std::stringstream strm(" Server { ");
	strm << "host = " << serv.getHost();
	strm << " port = " << serv.getPort();
	strm <<   "} : " << msg;
	std::string resMsg = msg.c_str();
	return fastLog(type, file , resMsg);

}
