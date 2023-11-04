#pragma once

#include "../../include/includes.hpp"

#include <map>

using namespace std;

class Location {

		private:
			template<class container, class value>	bool Add(container &c, value v);
			vector <string>  						Allowed_Method;
			vector <string> 						indexes;
			vector <string> 						try_file;
			vector <std::pair<int , string> >  		error_page;
			map<string, string>						cgi;
			string		  							root;
			string 		  							redirection_text;
			string 									default_type;
			string									path;
			int  			 						redirection_code;
			bool  									AutoIndex;
			bool 									_isRedirection;
			bool									upload;
		public :
				Location(string path);
				~Location();
				void final(); // this method resize all container
		/*********************************< adder >*******************/
				bool             AddErrorPage(const string &page, int code);
				bool             AddHttpMethod(const string &method);
				bool             AddTryFile(const string &file);
				bool             AddIndex(const string &index);
				bool 			AddCGI(string bin, string ex); // no impleneted

				/*********************************< setter >*******************/

						void         setRoot(const string &root);
						void         setAutoIndex(bool val);
						void         setRedirection(bool val);
						void         setDefaultType(const string &type);
						void         setRedirectionCode(int  code);
						void         setRedirectionText(string &text);
						void         setRedirection(int code , string &text);
						void         setPath(string &path);
						void		setUploadOn(bool b);// no impleneted
				/*********************************< getter >*******************/

				bool                      				getErrorPage(int code, string  &page)	const;
				const vector<std::pair<int,string> >    &getErrorPageList() 					const;
				const vector<string>					&getIndexesList()						const;
				const vector<string>					&getTryFiles()							const;
				const string                         	&getDefaultTypes() 						const;
				const string                			&getRoot() 								const;
				const string                      		&getPath() 								const;
				const string                 			getUri() 								const; // uri = root + path;
				const int 								getRedirectionCode()					const;
				const string 							&getRedirectionText()					const;
				const map<string, string>				&getCgi()								const;	// no implemented
				const string							getCgiBinFor(string file)				const;	// no implemented
		/*********************************< checker >*******************/
				bool isMethodAllowed(const string &method) 	const;
				bool isRedirection()						const;
				bool isAutoIndex()							const;
				bool isMatch(string uri)					const;
				bool isUploadOn()							const;	// no implemented
				bool isCgiExtention(string file) 			const;
		/*********************************< for Debug  >*******************/

		void printLocation();
};
