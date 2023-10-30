#include "../include/includes.hpp"
#include <fstream>


#define ALLOWED_URL_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%"

class RequestBuffer{

	private:

		std::string									FirstLine;
		std::string									Headers;
		int											level;
		std::string									BodyPath;
		int											Max_BodySize;
		bool										Post;
		int 										Body_offset;
		int											content_length;
		int											Body_insertion_type;
		int											Chunked_size;

	public:

		RequestBuffer(int Max_BodySize);
		RequestBuffer(const RequestBuffer &x);
		RequestBuffer &operator=(const RequestBuffer &x);
		~RequestBuffer();

	public: // Getters.

		std::string									const &getBody() const;
		std::string									const &getFirstLine() const;
		std::string									const &getHeaders() const;
		int											const &getLevel() const;
		int											const &getMax_BodySize() const;

	public: // Setters. Not sure if we need them.

		void										setFirstLine(std::string const &FirstLine);
		void										setHeaders(std::string const &Headers);
		void										setLevel(int const &level);
		void										setBody(char* const &Body);
		void										setMax_BodySize(int const &Max_BodySize);

	public:

		int											insertToBuffer(char *buffer, int size);


	private: // Tools: Those member function will be used inside the class only.
		void										insertFirstLine(char *buffer, int size);
		void										insertHeaders(char *buffer, int size);
		void										insertBody(char *buffer, int size);
		void										insertBodyChunked(char *buffer, int size);
		void										insertBodyContentLength(char *buffer, int size);
		void										insertBodyBoundary(char *buffer, int size);
		std::string									GenerateBodyPath();
		void										parseFirstLine();
		int											getChunkedSize(char *buffer, int size);
};
