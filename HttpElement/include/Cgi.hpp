#pragma once
#include "../../include/includes.hpp"
#include "Request.hpp"

#include <string>
using namespace std;

struct s_cgiInfo
{
	int code;
	int pid;
	string output;
	string input;
};

typedef struct s_cgiInfo t_cgiInfo;

class Cgi
{
private:

public:
	static	t_cgiInfo INTERNAL_ERROR;
	static	t_cgiInfo  Run(Request &req, std::string  &bin, std::string &path);
	static	char **makeEnv(Request &req, std::string &path);
	static	char **MakeArgs(string &bin, string &path);
	static	bool toEnvVariable(const string &name, const string &value, string &tmp);
	static	bool makeStaticVariable(char **env, Request &req, string &path);
	static	void cgiProcess(t_cgiInfo &info, Request &req, char **env,  char **args);
	static	string getRandomName(string root="/tmp/", string postfix="-file");
	static	char **deleteDP(char **pt, size_t pos);
};




