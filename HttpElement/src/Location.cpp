
#include "../include/Location.hpp"
using namespace std;

/*********************************< helper >*******************/
template<class container, class value>	bool Location::Add(container &c, value v)
{
	if (std::find(c.begin(), c.end(), v) != c.end()) return false;
		c.push_back(v);
	return true;
}

Location::Location(string path): path(path)
{
	this->redirection_code = 200;
	this->redirection_text = "ok";
	this->AutoIndex = false;
	this->upload = false;
	this->_isRedirection = false; // add 
	this->isDown = false;
	this->upload_path = "";
	this->_isRedirection = false;
}
Location::~Location(){}
void Location::final()
{
	if (this->default_type.empty())
		this->default_type = "application/octet-stream";
	Allowed_Method.shrink_to_fit();
	indexes.shrink_to_fit();
	error_page.shrink_to_fit();
}
/*********************************< adder >*******************/

bool	Location::AddErrorPage(const string &page, int code) 		{	return Add(error_page, std::make_pair(code, page)); }
bool    Location::AddHttpMethod(const string &method)				{	return Add(Allowed_Method, method);   }
bool    Location::AddIndex(const string &index)   					{	return Add(indexes, index);    }
bool 	Location::AddCGI(string bin, string ex)
{
	if(cgi.find(ex) != cgi.end())
		return false;
	cgi.insert(std::make_pair(ex, bin));
	 return true;
}

/*********************************< setter >*******************/

void	Location::setRoot(const string &root)                  {   this->root = root;                                   	 }
void	Location::setAutoIndex(bool val)                       {   this->AutoIndex = val;                               	 }
void	Location::setRedirection(bool val)                     {   this->_isRedirection = val;                          	 }
void	Location::setDefaultType(const string &type)           {   this->default_type = type;                           	 }
void	Location::setRedirectionCode(int  code)                {   this->redirection_code = code;                       	 }
void	Location::setRedirectionText(string &text)             {   this->redirection_text = text;                        	}
void	Location::setRedirection(int code, string &text)       {   setRedirectionCode(code) , setRedirectionText(text);  	}
void	Location::setPath(string &path)                        {   this->path = path;                                     	}
void	Location::setUploadOn(bool b)						   {   this->upload = b;                                      	}
void	Location::setUploadPath(const string	&upload_path)	{	this->upload_path = upload_path;						}
void 	Location::setDownload(bool b) 							{	 this->isDown = b;										}


/*********************************< getter >*******************/

	bool	Location::getErrorPage(int code, string  &page)   const
	{
		std::vector<std::pair<int,string> >::const_iterator it = error_page.begin();
		page = "";
		while (it != error_page.end())
			if (it->first == code)
				break;
		page = it->second;
		return   it == error_page.end() ?  false  :  true;
	}

const vector<std::pair<int,string> >    &Location::getErrorPageList()   const    { return    error_page;          }
const vector<string>					&Location::getIndexesList()		const 	 { return indexes; 				  }
const string							&Location::getDefaultTypes()    const    { return    default_type;        }
const string							&Location::getRoot()            const    { return    root;                }
const string                      		&Location::getPath() 		    const    { return    path;                }
const string                 			Location::getUri() 		        const    { return    root + path;         }
	int 								Location::getRedirectionCode()  const    {		return redirection_code; }
const string 						    &Location::getRedirectionText()  const   {  return redirection_text;    }
const string							Location::getCgiBinFor(string file) const
{
	size_t pos = file.find(".");
	string ext;
	if (pos  == string::npos) return "";
		ext = file.substr(pos);
		if ( cgi.find(ext) != cgi.end() )
			return    cgi.at(ext);
		else
			return "";
}

const string							&Location::getUploadPath()	const{ return  this->upload_path; }
/*********************************< checker >*******************/

bool Location::isMethodAllowed(const string &method) const          {   return (std::find(Allowed_Method.begin(), Allowed_Method.end() , method) != Allowed_Method.end()); }
bool Location::isRedirection()                       const          {   return _isRedirection; }
bool Location::isAutoIndex()                         const          {   return AutoIndex; }
bool  Location::isMatch(string uri)    const
{
	if (uri.size() < this->path.size())  return false;
	else if (uri == path) return true;
	size_t pos = uri.find(this->path ,0);
	if (pos != 0) return false;
	if (this->path.find_last_of("/") == this->path.size() - 1) return true;
	return uri.at(this->path.size()) == '/' ;
}
bool Location::isUploadOn()					const          { return upload; }
bool Location::isDownloadEnable()			const			{ return  isDown; }
bool Location::isCgiExtention(string file)	const
{
	size_t pos = file.find(".");
	string ext;
	if (pos  == string::npos) return false;
		ext = file.substr(pos);
	return cgi.find(ext) != cgi.end();
}

/*********************************< for Debug >*******************/
	void Location::printLocation() {
		std::cout << "Allowed_Method: ";
		for (std::vector<std::string>::const_iterator it = Allowed_Method.begin(); it != Allowed_Method.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;

		std::cout << "Indexes: ";
		for (std::vector<std::string>::const_iterator it = indexes.begin(); it != indexes.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;

		std::cout << "Error_Page: ";
		for (std::vector<std::pair<int, std::string> >::const_iterator it = error_page.begin(); it != error_page.end(); ++it) {
			std::cout << "[" << it->first << ", " << it->second << "] ";
		}
		std::cout << std::endl;

		std::cout << "Root: " << root << std::endl;
		std::cout << "Redirection_Text: " << redirection_text << std::endl;
		std::cout << "Default_Type: " << default_type << std::endl;
		std::cout << "Path: " << path << std::endl;
		std::cout << "Redirection_Code: " << redirection_code << std::endl;
		std::cout << "AutoIndex: " << (AutoIndex ? "true" : "false") << std::endl;
		std::cout << "_isRedirection: " << (_isRedirection ? "true" : "false") << std::endl;
	}
