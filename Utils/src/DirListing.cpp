#include "../include/DirListing.hpp"
#include  <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>

std::string DirListing::GenerateFileRow(const std::string &parent, const std::string &name)
{
	std::stringstream output;
	std::string path = parent + "/" + name;
	output << "<tr><td>\n<a href=\"";
	output << name;
	output << "\">";
	output << name;
	output << "</a> </td>";

	char buff[100] = "";
	struct stat info;
	if (stat(path.c_str(), &info) == 0)
	{
		strftime(buff, 100, "%d/%m/%Y %H:%M:%S", localtime(&info.st_mtime));
		output << " <td>" <<  info.st_size << " byte</td>\n<td>" << buff << "</td>\n</tr>";
	}
	else 
	    output << " <td> -- </td>\n<td> --:--:-- --:-- </td>\n</tr>";
	return output.str();
}

std::string DirListing::MakeHtml(std::string DirName, std::string HtmlContent)
{
	std::stringstream html;
	html << "<html>\n<head>";
	html <<  "<style>\nbody{ \n ;text-indent: 10%; \n color : rgb(200, 200, 255); \n background-color :black;}";
	html <<  "th, td{\npadding-left: 50px;\npadding-bottom: 10px;\ncolor : rgb(200, 200, 200);\n}\n</style>";
	html <<  "<title> Index of " << DirName << "</title>\n</head>\n<body><center>\n";
	html << "<h1> Index of " << DirName <<"</h1>";
	html <<   "<hr>\n<table>\n<tr>\n<th> Name </th>\n<th> Size  </th>\n<th>Last Modified </th>\n</tr>";
	html << HtmlContent;
	html << "   </table>\n<h2>Hustlers Server 1.0 [beta] </h2>\n</center>\n</body></html>";
	return html.str();
}

bool DirListing::getDirlistigHtml(const std::string path, std::string &output)
{
    DIR *dir = NULL;
    dirent *ent = NULL;
    dir  = opendir(path.c_str());
	std::string htmlContent;
    if (dir == NULL) return false;
    while ( (ent =  readdir(dir)) != NULL)
    {	
		if(ent->d_name[0] != '.')
		htmlContent += GenerateFileRow(path , ent->d_name);
    }
	output = MakeHtml(path, htmlContent);
     closedir(dir);
	 return true;
}
