// #include "RequestBuffer.hpp"

// /***************************************************************/
// RequestBuffer::RequestBuffer(int Max_BodySize){
// 	this->Max_BodySize = Max_BodySize;
// 	this->Body = new char[Max_BodySize + 1];
// 	this->level = 0;
// }

// RequestBuffer::RequestBuffer(const RequestBuffer &x){
// 	this->Max_BodySize = x.Max_BodySize;
// 	this->Body = new char[Max_BodySize + 1];
// 	this->level = x.level;
// }

// RequestBuffer &RequestBuffer::operator=(const RequestBuffer &x){

// 	if (this == &x)
// 		return (*this);
// 	if (this->Body)
// 		delete[] this->Body;
// 	this->Max_BodySize = x.Max_BodySize;
// 	this->Body = new char[Max_BodySize];
// 	this->level = x.level;
// 	return *this;
// }

// RequestBuffer::~RequestBuffer() {delete[] this->Body;}
// /***************************************************************/


// /*                           Getters                           */
// /***************************************************************/
// std::string const &RequestBuffer::getFirstLine() const		{return this->FirstLine;}
// std::string const &RequestBuffer::getHeaders() const		{return this->Headers;}
// int const &RequestBuffer::getLevel() const					{return this->level;}
// char* const &RequestBuffer::getBody() const					{return this->Body;}
// int const &RequestBuffer::getMax_BodySize() const			{return this->Max_BodySize;}
// /***************************************************************/


// /*                           Setters                            /
// /***************************************************************/
// void RequestBuffer::setFirstLine(std::string const &FirstLine)	{this->FirstLine = FirstLine;}
// void RequestBuffer::setHeaders(std::string const &Headers)		{this->Headers = Headers;}
// void RequestBuffer::setLevel(int const &level)					{this->level = level;}
// void RequestBuffer::setBody(char* const &Body)					{this->Body = Body;}
// void RequestBuffer::setMax_BodySize(int const &Max_BodySize)	{this->Max_BodySize = Max_BodySize;}
// /***************************************************************/



// /*                           Methods                           */
// /***************************************************************/

// /*	RequestBuffer::insertToBuffer:




// */
// void RequestBuffer::insertToBuffer(char *buffer, int size)
// {
// 	switch (this->level) {


// 	}


// }

// /***************************************************************/
