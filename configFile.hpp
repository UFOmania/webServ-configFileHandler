#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <iostream>
#include <ctype.h>
#include <sstream>
#include <vector>
#include <iterator>
#include <fstream>


#include "classes/ServerConfig.hpp"
#include "classes/ConfigFileTypes.hpp"


#include "classes/ConfigException.hpp"

#define RESET   "\033[0m"
#define BOLD    "\033[1m"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"



void normalizeConfigFormat(std::string &conf);
void validateShape(Configtokens &t);
void tokenizeConfig(std::string & conf, Configtokens & tokens);
bool validateBlocks( std::string & conf);
void putErr(std::string msg);
bool isWord(std::string & s);
void parseConfig(Configtokens & tokens, std::vector<ServerConfig> & servers);
void validateServer(serverToken_t &server, ServerConfig & s);


#endif