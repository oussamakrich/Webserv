
#include "../../include/Location.hpp"
using namespace std;

/*********************************< helper >*******************/
template<class container, class value>	bool Location::Add(container c, value v)
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
	this->default_type = "Al";
}
Location::~Location(){}
void Location::final()
{
	Allowed_Method.shrink_to_fit();			
	indexes.shrink_to_fit(); 
	try_file.shrink_to_fit();
	error_page.shrink_to_fit();
} 
/*********************************< adder >*******************/

bool	Location::AddErrorPage(const string &page, int code) 		{ return Add(error_page, std::make_pair(code, page)); }
bool    Location::AddHttpMethod(const string &method)				{  return Add(Allowed_Method, method);   }
bool    Location::AddTryFile(const string &file) 					{ return Add(try_file, file);    }
bool    Location::AddIndex(const string &index)   					{ return Add(indexes, index);    }

/*********************************< setter >*******************/

void	Location::setRoot(const string &root)                  {   this->root = root;                                    }
void	Location::setAutoIndex(bool val)                       {   this->AutoIndex = val;                                }
void	Location::setRedirection(bool val)                     {   this->_isRedirection = val;                           }
void	Location::setDefaultType(const string &type)           {   this->default_type = type;                            }
void	Location::setRedirectionCode(int  code)                {   this->redirection_code = code;                        }
void	Location::setRedirectionText(string &text)             {   this->redirection_text = text;                        } 
void	Location::setRedirection(int code, string &text)       {   setRedirectionCode(code) , setRedirectionText(text);  }
void	Location::setPath(string &path)                        {  this->path = path;                                     }

/*********************************< getter >*******************/

	bool     Location::getErrorPage(int code, string  &page)   const
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
const vector<string>					&Location::getTryFiles()		const	 { return try_file;				  }
const string                         	&Location::getDefaultTypes()    const    { return    default_type;        }
const string                			&Location::getRoot()            const    { return    root;                }
const string                      		&Location::getPath() 		    const    { return    path;                }
const string                 			Location::getUri() 		        const    { return    root + path;         }

/*********************************< cheker >*******************/

bool Location::isMethodAllowed(const string &method) const          {   return (std::find(Allowed_Method.begin(), Allowed_Method.end() , method) != Allowed_Method.end()); }
bool Location::isRedirection()                       const          {   return _isRedirection; } 
bool Location::isAutoIndex()                         const          {   return AutoIndex; }