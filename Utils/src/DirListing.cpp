#include "../include/DirListing.hpp"
#include "../../include/includes.hpp"
#include <dirent.h>

std::string DirListing::GenerateFileRow(const std::string &parent, const std::string &name, std::string vt_path)
{
	std::stringstream output;
	std::string path = parent + "/" + name;
	std::string path2 = vt_path + "/" + name;
	removeSlash(path);
	removeSlash(path2);
	struct stat info;
	int res_stat = stat(path.c_str(), &info);
	output << "<tr><td>\n<a href=\"";
	output << path2;
	output <<  (S_ISDIR(info.st_mode) ?  "/" : "");
	output <<  "\">";
	output << name;
	output << "</a> </td>";

	char buff[100] = "";

	if (res_stat == 0)
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
	html <<  "<style>\nbody{ \n ;text-indent: 10%; \n color : #F5F5F5; \n background-color :#363062;}";
	html <<  "th, td{\npadding-left: 50px;\npadding-bottom: 10px;\ncolor : #F99417;\n}";
	html <<  "\na {color:#F99417;text-decoration: none;border-bottom: 2px dashed black:}\n";
	html <<  "\n a:hover {border-bottom: 2px solid white; color : white;}\n";
	html <<  "\n hr { color :#F99417;}\n";
	html << "\n</style>";
	html <<  "<title> Index of " << DirName << "</title>\n</head>\n<body><center>\n";
	html << "<h1> Index of " << DirName <<"</h1>";
	html <<   "<hr>\n<table>\n<tr>\n<th> Name </th>\n<th> Size  </th>\n<th>Last Modified </th>\n</tr>";
	html << HtmlContent;
	html << "</table>\n<h2>Hustlers Server 1.0 [beta] </h2>\n</center>\n</body></html>";
	return html.str();
}

bool DirListing::getDirlistigHtml(const std::string path, std::string &output, std::string vt_path)
{
    DIR *dir = NULL;
    dirent *ent = NULL;
    removeSlash(vt_path);
    dir  = opendir(path.c_str());
	std::string htmlContent = "";
    if (dir == NULL) return false;
    while ( (ent =  readdir(dir)) != NULL)
    {
		if(ent->d_name[0] != '.')
		htmlContent += GenerateFileRow(path , ent->d_name, vt_path);
    }
	output = MakeHtml(vt_path, htmlContent);
     closedir(dir);
	 return true;
}
