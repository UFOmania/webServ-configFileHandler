#include "configFile.hpp"

void putErr(std::string msg)
{
	std::cerr << RED << "Error:" << msg << RESET << std::endl;
}

bool isSpecialToken(std::string & token)
{
    return token == "}" || token == "{" || token == ";";
}

bool isWord(std::string & s)
{
	return s != "{" && s != "}" && s != ";"; 
}

bool isNumber(std::string & s)
{
	int i = 0;
	char * end;

	i = std::strtol(s.c_str(), &end, 10);
	if (*end != '\0')
		return false;
	return true;
}