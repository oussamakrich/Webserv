#pragma once

#include "../../include/includes.hpp"


using namespace std;

class Location {

private:
	template<class container, class value>	bool Add(container &c, value v);
	vector <string>  						Allowed_Method;
	vector <string> 						indexes;
	vector <std::pair<int , string> >  		error_page;
	map<string, string>						cgi;
	string		  							root;
	string 		  							redirection_text;
	string 									default_type;
	string									path;
	string									upload_path; // new_add
	int  			 						redirection_code;
	bool  									AutoIndex;
	bool 									_isRedirection;
	bool									upload;
	bool									isDown;
public :
		Location(string path);
		~Location();
		void final(); // this method resize all container
		
/*********************************< adder >*******************/

		bool             AddErrorPage(const string &page, int code);
		bool             AddHttpMethod(const string &method);
		bool             AddIndex(const string &index);
		bool 			AddCGI(string bin, string ex);

/*********************************< setter >*******************/

		void         setRoot(const string &root);
		void         setAutoIndex(bool val);
		void         setRedirection(bool val);
		void         setDefaultType(const string &type);
		void         setRedirectionCode(int  code);
		void         setRedirectionText(string &text);
		void         setRedirection(int code , string &text);
		void         setPath(string &path);
		void		setUploadOn(bool b);
		void 		setUploadPath(const string &upload_path);
		void 		setDownload(bool b); 

		/*********************************< getter >*******************/

		bool                      				getErrorPage(int code, string  &page)	const;
		const vector<std::pair<int,string> >    &getErrorPageList() 					const;
		const vector<string>					&getIndexesList()						const;
		const string                         	&getDefaultTypes() 						const;
		const string                			&getRoot() 								const;
		const string                      		&getPath() 								const;
		const string                 			getUri() 								const; // uri = root + path;
		int 									getRedirectionCode()					const;
		const string 							&getRedirectionText()					const;
		const map<string, string>				&getCgi()								const;
		const string							getCgiBinFor(string file)				const;
		const string							&getUploadPath()						const; // new_add
/*********************************< checker >*******************/
		bool isMethodAllowed(const string &method) 	const;
		bool isRedirection()						const;
		bool isAutoIndex()							const;
		bool isMatch(string uri)					const;
		bool isUploadOn()							const;
		bool isCgiExtention(string file) 			const;
		bool isDownloadEnable()						const;	
/*********************************< for Debug  >*******************/

};
