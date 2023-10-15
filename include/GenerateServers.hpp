#include "includes.hpp"
#include "Server.hpp"

class GenerateServers{
	public:
		GenerateServers();
		~GenerateServers();
		void Generator(std::vector<TOKEN_PAIR> &tokens, std::vector<Server*> &servers);
		GenerateServers(const GenerateServers &copy);
		GenerateServers &operator=(const GenerateServers &copy);
		int getValueType(std::vector<TOKEN_PAIR>::iterator &i);
	private:
		int switcher;
};
