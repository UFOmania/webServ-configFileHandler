#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include "Location.hpp"
#include "ConfigFileTypes.hpp"

class ServerConfig
{
    private:
        std::vector < errorPages_t >    _error_pages;
        std::vector <cgiPass_t>         _cgi_pass;
        std::vector < class Location >  _locations;
        
        std::string _server_name;
        std::string _root;
        std::string _index;
        size_t _client_body_size;
        int _listen;
        

    public:
        ServerConfig();
        ~ServerConfig();
        ServerConfig(const ServerConfig & s);

        ServerConfig &operator=( const ServerConfig & s);

        void SetServerName(const std::string & val);
        void SetListen(const int & val);
        void SetRoot(const std::string & val);
        void SetIndex(const std::string & val);
        void SetClientBodySize(const size_t & val);
        void AddErrorPage(const errorPages_t & val);
        void AddLocation(const Location & location);
        void AddCgiPass(const cgiPass_t & cgi);
        Location *EditLocation(int idx);

        const std::string &GetRoot() const ;
        const std::string &GetIndex() const ;
        void PrintServerInfo() const;
};


#endif