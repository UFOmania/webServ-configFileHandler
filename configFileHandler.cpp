#include "configFile.hpp"


bool configFileHandler(const char * conf, std::vector<ServerConfig> & servers)
{

	std::ifstream file(conf);

	std::string ss;
	std::string s;
	while (std::getline(file, s))
		ss +=  s + "\n";

	file.close();


	try
	{
		Configtokens tokens;

		normalizeConfigFormat(ss);
		validateBlocks(ss);
		tokenizeConfig(ss, tokens);
		validateShape(tokens);
		parseConfig(tokens, servers);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;
}









