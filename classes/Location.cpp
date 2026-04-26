#include "Location.hpp"


Location::~Location() {}

Location::Location() : 
    _allowGET(false), 
    _allowPOST(false), 
    _allowDELETE(false), 
    _autoindex(false), 
    _client_body_size(1000000)
{

}

Location::Location(const Location & l)
{
    if (&l != this)
        *this = l;
}

Location &Location::operator=(const Location & l)
{       
    _path = l._path;
    _index = l._index;
    _root = l._root;
    _upload_path = l._upload_path;
    _client_body_size = l._client_body_size;
    _autoindex = l._autoindex;
    _allowDELETE = l._allowDELETE;
    _allowGET = l._allowGET;
    _allowPOST = l._allowPOST;
    return *this;
}

void Location::AddRedirection(const redirection_t & red)            {   _redirections.push_back(red);   }
void Location::SetPath(const std::string & val)                     {   _path = val;                    }
void Location::SetIndex(const std::string & val)                    {   _index = val;                   }
void Location::SetRoot(const std::string & val)                     {   _root = val;                    }
void Location::SetUploadPath(const std::string & val)               {   _upload_path = val;             }
void Location::SetClientBodySize(const size_t & val)                {   _client_body_size = val;             }
void Location::SetAutoindex(const bool & val)                       {  _autoindex = val;             }
void Location::SetAllowGet(const bool & val)                        {   _allowGET = val;             }
void Location::SetAllowPost(const bool & val)                       {   _allowPOST = val;             }
void Location::SetAllowDelete(const bool & val)                     {   _allowDELETE = val;             }
const std::string &Location::GetRoot() const                        {   return _root;                   }
const std::string &Location::GetPath() const                        {   return _path;                   }


#include <iostream>
void Location::PrintLocationInfo(size_t index) const
{
    std::cout << "\n  Location #" << index << std::endl;
    std::cout << "  " << std::string(54, '-') << std::endl;
    std::cout << "    Path:              " << _path << std::endl;
    std::cout << "    Root:              " << _root << std::endl;
    std::cout << "    Index:             " << _index << std::endl;
    std::cout << "    Upload Path:       " << _upload_path << std::endl;
    std::cout << "    Max Body Size:     " << _client_body_size << " bytes" << std::endl;
    std::cout << "    Autoindex:         " << (_autoindex ? "ON" : "OFF") << std::endl;
    
    std::cout << "    Allowed Methods:   ";
    std::vector<std::string> methods;
    if (_allowGET)    methods.push_back("GET");
    if (_allowPOST)   methods.push_back("POST");
    if (_allowDELETE) methods.push_back("DELETE");
    
    if (methods.empty())
    {
        std::cout << "(None)" << std::endl;
    }
    else
    {
        for (size_t i = 0; i < methods.size(); i++)
        {
            std::cout << methods[i];
            if (i < methods.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    // Redirections
    if (!_redirections.empty())
    {
        std::cout << "    Redirections:      " << std::endl;
        for (size_t i = 0; i < _redirections.size(); i++)
        {
            std::cout << "      " << i + 1 << ". " << _redirections[i].new_path 
                      << " (Status: " << _redirections[i].status << ")" << std::endl;
        }
    }
}

