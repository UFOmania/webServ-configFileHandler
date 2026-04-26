
#include "ConfigException.hpp"

ConfigException::ConfigException(){}
ConfigException::~ConfigException() throw () {}
ConfigException::ConfigException(std::string msg) : _msg(std::string( "Error: " + msg)) {}

const char * ConfigException::what() const throw() 
{
    return _msg.c_str();
}
