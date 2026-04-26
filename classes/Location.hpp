#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include "ConfigFileTypes.hpp"

class Location
{
    private:
        std::vector <redirection_t> _redirections;
        
        std::string _path;
        std::string _index;
        std::string _root;
        std::string _upload_path;
        size_t      _client_body_size;
        bool        _autoindex;
        bool        _allowGET;
        bool        _allowPOST;
        bool        _allowDELETE;
        

    public:
        Location();
        ~Location();
        Location(const Location & s);
        Location &operator=( const Location & s);

        void AddRedirection(const redirection_t & red);
        void SetPath(const std::string & val);
        void SetIndex(const std::string & val);
        void SetRoot(const std::string & val);
        void SetUploadPath(const std::string & val);
        void SetClientBodySize(const size_t & val);
        void SetAutoindex(const bool & val);
        void SetAllowGet(const bool & val);
        void SetAllowPost(const bool & val);
        void SetAllowDelete(const bool & val);

        const std::string &GetRoot() const;
        const std::string &GetPath() const;
        void PrintLocationInfo(size_t index) const;

        
};


#endif