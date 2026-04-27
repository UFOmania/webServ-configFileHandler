#include "configFile.hpp"

void tokenizeConfig(std::string & conf, Configtokens & tokens)
{
    std::stringstream ss(conf);
    std::string str;

    while (ss >> str)
    {
        tokens.push_back(str);
    }
}