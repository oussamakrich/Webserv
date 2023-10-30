 
#pragma once
#include "../../HttpElement/include/Request.hpp"
#include "../../Parsing/include/GenerateLocation.hpp" // use  for Isinger() function
#include "../../Request/RequestBuffer.hpp"



#define ALLOWED_URL_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%"
// temp class for code
// class RequestBuffer{
//
// 	private:
//
// 		std::string									FirstLine;
// 		std::string									Headers;
// 		int											level;
// 		std::string									BodyPath;
// 		std::ifstream								BodyFile;
// 		int											Max_BodySize;
// 		bool										Post;
// 		int 										Body_offset;
// 		int											content_length;
// 		int											Body_insertion_type;
// 		int											Chunked_size;
//
// 	public:
//
// 		RequestBuffer(int Max_BodySize);
// 		RequestBuffer(const RequestBuffer &x);
// 		RequestBuffer &operator=(const RequestBuffer &x);
// 		~RequestBuffer();
//
// 	public: // Getters.
//
// 		std::string									const &getBody() const;
// 		std::string									const &getFirstLine() const;
// 		std::string									const &getHeaders() const;
// 		int											const &getLevel() const;
// 		int											const &getMax_BodySize() const;
// 		
// };

class ParsRequest
{
public :
		static Request *Pars(RequestBuffer &reqBuff);
		static void ParsFirstLine(Request& req, std::string line) ;
		static bool isValidKey(std::string key);
		static bool ParsHeaders(Request& req, std::string& line);
};

//***************************** util ***************************************
std::string replaceAll(std::string str, const std::string s1, const std::string s2);
std::vector<std::string> split(const std::string& str, char delimiter);
int getMethodCode(std::string  method);

