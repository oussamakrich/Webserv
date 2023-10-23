#pragma once 
#include "includes.hpp"
#include <vector>
#include <string>

using namespace std;

class Location {

		private:
			template<class container, class value>	bool Add(container c, value v); 
			vector <string>  						Allowed_Method;			
			vector <string> 						indexes; 
			vector <string> 						try_file;
			vector <std::pair<int , string> >  		error_page;
			string		  							root;                       			
			string 		  							redirection_text;
			string 									default_type; 			
			string									path;
			int  			 						redirection_code; 	
			bool  									AutoIndex;
			bool 									_isRedirection;	
		public :
				Location(string path);
				~Location();
				void final(); // this method resize all container 
		/*********************************< adder >*******************/
				bool             AddErrorPage(const string &page, int code);
				bool             AddHttpMethod(const string &method);
				bool             AddTryFile(const string &file); 
				bool             AddIndex(const string &index);

		/*********************************< setter >*******************/

				void         setRoot(const string &root);
				void         setAutoIndex(bool val);
				void         setRedirection(bool val);
				void         setDefaultType(const string &type);
				void         setRedirectionCode(int  code);
				void         setRedirectionText(string &text);
				void         setRedirection(int code , string &text); 
				void         setPath(string &path);

		/*********************************< getter >*******************/

				bool                      				getErrorPage(int code, string  &page)	const;
				const vector<std::pair<int,string> >    &getErrorPageList() 					const;
				const string                         	&getDefaultTypes() 						const;
				const string                			&getRoot() 								const;
				const string                      		&getPath() 								const;
				const string                 			getUri() 								const; // uri = root + path;
		/*********************************< cheker >*******************/
				bool isMethodAllowed(const string &method) 	const;
				bool isRedirection()						const; 
				bool isAutoIndex()							const;
		
};
