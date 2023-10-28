#include "../include/includes.hpp"

class RequestBuffer{

	private:

		std::string									FirstLine;
		std::string									Headers;
		int											level;
		char*										Body;
		int											Max_BodySize;

	public:

		RequestBuffer(int Max_BodySize);
		RequestBuffer(const RequestBuffer &x);
		RequestBuffer &operator=(const RequestBuffer &x);
		~RequestBuffer();

	public: // Getters.

		std::string									const &getFirstLine() const;
		std::string									const &getHeaders() const;
		int											const &getLevel() const;
		char*										const &getBody() const;
		int											const &getMax_BodySize() const;

	public: // Setters. Not sure if we need them.

		void										setFirstLine(std::string const &FirstLine);
		void										setHeaders(std::string const &Headers);
		void										setLevel(int const &level);
		void										setBody(char* const &Body);
		void										setMax_BodySize(int const &Max_BodySize);

	public:

		void										insertToBuffer(char *buffer, int size);
		char*										getBodyRange(int start, int end);
		char*										getBodyRange(int start);

};
