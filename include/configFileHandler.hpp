#ifndef CONFIGFILEHANDLER_HPP
#define CONFIGFILEHANDLER_HPP

#include <vector>

#include "../classes/ServerConfig.hpp"

bool configFileHandler(const char * conf, std::vector<ServerConfig> & servers);


#endif