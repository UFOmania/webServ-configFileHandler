#ifndef CONFIGEXCEPTION_HPP
#define CONFIGEXCEPTION_HPP

#include <exception>
#include <string>



class ConfigException : public std::exception
{
    private:
        std::string _msg;
    public:
        ~ConfigException() throw() ;
        ConfigException();
        ConfigException(std::string msg);
        const char * what() const throw() ;
};


#endif