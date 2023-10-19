#pragma once
#include "includes.hpp"
#include <fstream>

class Tokenizer {

	public:
		static std::vector<TOKEN> tokenGenerator(std::ifstream &file);
		Tokenizer();
		~Tokenizer();
		Tokenizer(const Tokenizer &copy);
		Tokenizer &operator=(const Tokenizer &copy);
};

