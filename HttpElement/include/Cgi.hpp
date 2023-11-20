#pragma once
#include "../../include/includes.hpp"
#include "Request.hpp"

using namespace std;

#define STATIC_VAR_NUM 7

#define CGITIME_OUT 60 
struct s_cgiInfo
{
	int code;
	int pid;
	string output;
	string input;
	long long time;
};

typedef struct s_cgiInfo t_cgiInfo;

class Cgi
{
	
#define 	INTERNAL_ERROR  InternalError();
private:

public:
		
	static	t_cgiInfo	Run(Request &req, std::string  &bin, std::string &path);
	static	char **makeEnv(Request &req, std::string &path);
	static	char **MakeArgs(string &bin, string &path);
	static	bool toEnvVariable(const string &name, const string &value, string &tmp);
	static	bool makeStaticVariable(char **env, Request &req, string &path);
	static	void cgiProcess(t_cgiInfo &info, Request &req, char **env,  char **args);
	static	string getRandomName(string root="/tmp/", string postfix="-file");
	static	char **deleteDP(char **pt, size_t pos);
	static	bool isFinished(t_cgiInfo &info, int &status);
	static	void CgiUnlink(t_cgiInfo info);
	static	bool  isTimeOut(t_cgiInfo &info);
	static	bool KillCgi(t_cgiInfo &info);
	static	t_cgiInfo InternalError(); 

};




