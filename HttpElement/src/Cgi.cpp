//
# include "../include/Cgi.hpp"
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <libgen.h>


t_cgiInfo Cgi::InternalError()
{
	t_cgiInfo t;
	t.code =  500;
	t.pid = -1;
	t.input = "";
	t.output = "";
	t.time = 0;
	return t;
}

t_cgiInfo  Cgi::Run(Request &req, std::string  &bin, std::string &path)
{

	char **env = makeEnv(req, path);
	char **args = NULL;
	t_cgiInfo info;
	if (env == NULL) return Cgi::INTERNAL_ERROR;
	args = MakeArgs(bin , path);
	if(args == NULL)
	{
		deleteDP(env, INT_MAX);
		return Cgi::INTERNAL_ERROR;
	}

	info.output = getRandomName("/tmp/", "-cgi-output"); //TODO: select root , default rot is /tmp
	info.input  = req.getBody(); //TODO: select root , default rot is /tmp
	info.code  	= 0;
	info.time	=  time(NULL);
	info.pid =  fork();
	if (info.pid == -1)
	{
		deleteDP(env  , INT_MAX);
		deleteDP(args , INT_MAX);
		std::cout << "Fork FAIL : ";
		return Cgi::INTERNAL_ERROR;
	}
	else if (info.pid == 0)
		cgiProcess(info, req,env,args);
	deleteDP(env  , INT_MAX);
	deleteDP(args , INT_MAX);
	return info;
}

string Cgi::getRandomName(string root, string postfix)
{
	try
	{
	
		timeval tm;
    	gettimeofday(&tm, nullptr);

		std::stringstream ss;
		ss <<root;
		ss << "/tmp-";
		ss <<tm.tv_sec << "@" << tm.tv_usec;
		ss << postfix;
		ss << ".bin";
		return ss.str();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return "";
	}

}


char **Cgi::deleteDP(char **pt, size_t pos)
{
	for(size_t i = 0; i < pos && pt[i] != NULL; i++)
		delete  pt[i];
	delete [] pt;
	return NULL;
}

void Cgi::cgiProcess(t_cgiInfo &info, Request &req, char **env,  char **args)
{
	try
	{
		int ifd = 0;
		int ofd = 0;

		char *parent = dirname(strdup(args[1]));
		if (chdir(parent) == -1) throw std::runtime_error("cgi  Cant Change Dir !");
		delete parent;
		if (req.getType()  == Request::POST)
		{
			ifd = open(info.input.c_str(), O_RDONLY, 0777);
			if (ifd == -1)	throw std::runtime_error("cgi  process can't creat input file");
			if (dup2(ifd, 0) == -1)			throw std::runtime_error("cgi poress cant dup stdinput wth input file");
			close(ifd);
		}
		ofd = open(info.output.c_str(),O_CREAT | O_RDWR | O_TRUNC,  0777);
		if (ofd == -1) 					throw std::runtime_error("cgi  process can't creat output File file");
		if (dup2(ofd, 1) == -1)			throw std::runtime_error("cgi poress cant dup staOutoutndrad : wth input file");
		if (execve(args[0], args, env) == -1)  throw std::runtime_error("cgi poress cant -> run ");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
			perror("erro : ");
		exit(-1);
	}
	exit(-1);
}

bool Cgi::makeStaticVariable(char **env, Request &req, string &path)
{
	try
	{
		string PATH_INFO = "PATH_INFO=";
		string CONTENT_LENGTH = "CONTENT_LENGTH=";
		string REQUEST_METHOD = "REQUEST_METHOD=";
		string QUERY_STRING = "QUERY_STRING=";
		string CONTENT_TYPE = "CONTENT_TYPE=";
		string SCRIPT_FILENAME = "SCRIPT_FILENAME=";
		PATH_INFO += path;
		REQUEST_METHOD += req.getMethod();
		QUERY_STRING += req.getQuery();
		CONTENT_TYPE += req.getHeader("Content-Type");
		CONTENT_LENGTH += req.getHeader("Content-Length");
		SCRIPT_FILENAME += path;
		env[0] = strdup(PATH_INFO.c_str());
		env[1] = strdup(CONTENT_LENGTH.c_str());
		env[2] = strdup(CONTENT_TYPE.c_str());
		env[3] = strdup(QUERY_STRING.c_str());
		env[4] = strdup(REQUEST_METHOD.c_str());
		env[5] = strdup("REDIRECT_STATUS=200");
		env[6] = strdup(SCRIPT_FILENAME.c_str());
		for(int i = 0; i < STATIC_VAR_NUM;i++)
			if(env[i] == NULL)
				return true;
		}
	catch(std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
		return false;
	}
	return true;
}


bool Cgi::toEnvVariable(const string &name, const string &value, string &tmp)
{
		try
		{
			tmp = "HTTP_";
			tmp += name;
			for (std::string::iterator it = tmp.begin(); it != tmp.end(); ++it)
			{
				if (*it == '-')
					*it = '_';
				else
					*it = std::toupper(*it);
			}
			tmp += "=";
			tmp += value;
			return true;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
				return false;
		}
}

char **Cgi::makeEnv(Request &req,std::string &path)
{
	char **env  = NULL;
	size_t pos = 0;
	size_t size = req.getHeaders().size() + STATIC_VAR_NUM + 1;

	try
	{

		string tmp;
		env = new(std::nothrow) char *[size];
		if (env == NULL) return NULL;
		map<string, string> header = req.getHeaders(); // TODO: use refernce ??
		map<string, string>::iterator it = header.begin();
		pos = STATIC_VAR_NUM;
		if (makeStaticVariable(env, req, path) == false)
			return deleteDP(env, pos);


		for(; it != header.end(); it++, pos++)
		{
			toEnvVariable(it->first, it->second, tmp);
			env[pos] = strdup(tmp.c_str());
			if (env[pos] == NULL)
				return deleteDP(env, pos);
		}
		env[size - 1] = NULL;
		return env;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
			return deleteDP(env, pos);
	}

}


char **Cgi::MakeArgs(string &bin, string &path)
{
	try
	{
		char **args = new char*[3];
		args[0] = strdup(bin.c_str());
		if (args[0] == NULL) return NULL;
		args[1] = strdup(path.c_str());
		if(args[1] == NULL) return deleteDP(args, 1);
		args[2] = NULL;
		return args;
	}
	catch(std::exception &ex)
	{
		 std::cout << ex.what();
		return NULL;
	}

}


bool Cgi::isFinished(t_cgiInfo &info, int &status)
{
	int s = 0;
	if (info.pid < 0) return true;
	if (waitpid(info.pid, &s, WNOHANG ) == 0)
		return false;
	if (WIFEXITED(s))
    status  = WEXITSTATUS(s);
	else
		status  = s + 128;
	return  true;
}

void Cgi::CgiUnlink(t_cgiInfo info)
{
	if (!access(info.input.c_str(), F_OK))
			unlink(info.input.c_str());
	if (!access(info.output.c_str(), F_OK))
			unlink(info.output.c_str());
}
bool Cgi::isTimeOut(t_cgiInfo &info)
{
	time_t  tm;
	time(&tm);
	return tm - info.time > CGITIME_OUT;
}

bool Cgi::KillCgi(t_cgiInfo &info)
{
	return kill(info.pid, SIGKILL) == 0;
}
