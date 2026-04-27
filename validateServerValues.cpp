#include "configFile.hpp"

void validateNumiricValues(serverToken_t & server, ServerConfig & s)
{
    char *end;
    size_t val;

    
    if (server.listen.empty())
        throw ConfigException("Invalid listen value >" + server.listen);
    val = std::strtol(server.listen.c_str(), &end, 10);
    if ((end && *end != '\0') || (val > 65535 || val < 1))
        throw ConfigException("Invalid listen value >" + server.listen);
    s.SetListen(val);

    
    if (!server.client_body_size.empty())
    {
        val = std::strtol(server.client_body_size.c_str(), &end, 10);
        if ((end && *end != '\0') || val < 0)
            throw ConfigException("Invalid client_body_size value >" + server.client_body_size);
        s.SetClientBodySize(val);
    }
    
    for(size_t j = 0; j < server.locations.size(); j++)
    {

        val = std::strtol(server.locations[j].client_body_size.c_str(), &end, 10);
        if ((end && *end != '\0') || val < 0)
            throw ConfigException("Invalid client_body_size value >" + server.locations[j].client_body_size);
        Location *location = s.EditLocation(j);
        if (location)
            location->SetClientBodySize(val);
    }

}

void validateboolValues(serverToken_t & server, ServerConfig & s)
{
    for(size_t i = 0; i < server.locations.size(); i++)
    {
        std::string auto_index = server.locations[i].auto_index;
        if (!auto_index.empty() && auto_index != "on" && auto_index != "off")
            throw ConfigException("Invalid auto_index value >" + auto_index);
        Location *location = s.EditLocation(i);
        if(location)
            location->SetAutoindex(auto_index == "on");
    }
}

#include <sys/stat.h> 

bool validateFile(std::string path, char type)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false;

    switch (type)
    {
        case 'f':
            if (!S_ISREG(info.st_mode)) return false;
            break;
        case 'd':
            if (!S_ISDIR(info.st_mode)) return false;
            break;
            
        case 0:
            if (!S_ISREG(info.st_mode) && !S_ISDIR(info.st_mode)) return false;
            break;

        default:
            return false;
    }
    return true;
}
void validateStringValues (serverToken_t & server, ServerConfig & s)
{
    //root
    if (server.root.empty())
        throw ConfigException("no root found");
    if (!validateFile(server.root, 'd'))
        throw ConfigException("Invalid root value >" + server.root);
    s.SetRoot(server.root);

    //index
    if (!validateFile((s.GetRoot() + server.index), 'f'))
        throw ConfigException("Invalid index value >" + server.index);
    s.SetIndex(s.GetRoot() + server.index);

    for(size_t i = 0; i < server.locations.size(); i++)
    {
        //dst
        Location * l = s.EditLocation(i);

        //location root
        if (!server.locations[i].root.empty())
        {
            if ( !validateFile(server.locations[i].root, 'd'))
               throw ConfigException("Invalid root value >" + server.locations[i].root);
        }
        else
            server.locations[i].root = s.GetRoot();
        l->SetRoot(server.locations[i].root);

        
        //path
        if (!server.locations[i].path.empty())
        {
            if ( !validateFile((l->GetRoot() + server.locations[i].path), 0))
                throw ConfigException("Invalid location path value >" + server.locations[i].path);
            l->SetPath(l->GetRoot() + server.locations[i].path);
        }

        //upload path
        if (!server.locations[i].upload_path.empty())
        {
            if ( !validateFile(l->GetRoot() + server.locations[i].upload_path, 'd'))
                throw ConfigException("Invalid upload_path value >" + server.locations[i].upload_path);
            l->SetUploadPath(l->GetRoot() + server.locations[i].upload_path);
        }

        //location index
        if (!server.locations[i].index.empty())
        {
            if ( !validateFile(l->GetPath() + server.locations[i].index, 'f'))
                throw ConfigException("Invalid location index value >" + server.locations[i].index);
            l->SetIndex(l->GetPath() + server.locations[i].index);
        }
        else
            l->SetIndex(s.GetIndex());
    }

}
void validateErrorPages (serverToken_t & server, ServerConfig & s)
{
    int i = 0;
    char *end;
    for(int i = 0; i < server.error_pages.size(); ++i)
    {
        errorPages_s e = server.error_pages[i];
        if (e.errorCode.length() != 3)
            throw ConfigException("Invalid error_page Code value >" + e.errorCode);
        
        i = std::strtol(e.errorCode.c_str(), &end, 10);
        if (!end || *end != '\0' || i < 100 || i > 599)
            throw ConfigException("Invalid error_page Code value >" + e.errorCode);
        e.pagePath = s.GetRoot() + "/" + e.pagePath;
        if (!validateFile(e.pagePath, 'f'))
            throw ConfigException("Invalid error_page path value >" + e.pagePath);

        //save to server
        s.AddErrorPage(e);
    }

}
void validateAllowMethods(serverToken_t & server, ServerConfig & s)
{
    for(int i = 0; i < server.locations.size(); i++)
    {
        int G = false,P = false,D = false;
        std::vector <std::string> & methods = server.locations[i].allow_methods;
        for (int j = 0; j < methods.size(); j++)
        {

            if (methods[j] == "GET")
            {
                if (G) //check for duplicated values
                    throw ConfigException("Dublicated Method >" + methods[j]);
                G = true;
            }
            else if (methods[j] == "POST")
            {
                if (P) //check for duplicated values
                    throw ConfigException("Dublicated Method >" + methods[j]);
                P = true;
            }
            else if (methods[j] == "DELETE")
            {
                if (D) //check for duplicated values
                    throw ConfigException("Dublicated Method >" + methods[j]);
                D = true;
            }
            else
                throw ConfigException("Invalid Method >" + methods[j]); 
        }

        //save to location
        Location * l = s.EditLocation(i);
        l->SetAllowGet(G);
        l->SetAllowPost(P);
        l->SetAllowDelete(D);
        
    }
}
void validateCgiPass(serverToken_t & server, ServerConfig & s)
{
        
    //check and save
    for (int j = 0; j < server.cgi_pass.size(); j++)
    {
        cgiPass_t cgi = server.cgi_pass[j];
        if (!validateFile(cgi.path, 'f'))
            throw ConfigException("Invalid cgi execution path >" + cgi.path);

        //save
        s.AddCgiPass(cgi);
    }

}
void validateRedirections(serverToken_t & server, ServerConfig & s)
{
    char *end;
    int i = 0;
    for (int i = 0; i < server.locations.size(); ++i)
    {
        //dst
        Location * l = s.EditLocation(i);

        //src
        redirection_t red = server.locations[i].redirection;

        if (red.status.empty() || red.new_path.empty())
            return;

        //check
        if (red.status.length() != 3)
            throw ConfigException("Invalid return Code value >" + red.status);

        i = std::strtol(red.status.c_str(), &end, 10);

        if (!end || *end != '\0' || i < 300 || i > 399)
            throw ConfigException("Invalid return Code value >" + red.status);

        if (!validateFile(l->GetRoot() + red.new_path, 'd'))
            throw ConfigException("Invalid return path value >" + red.new_path);

        //save
        l->AddRedirection(red);
    }
}


void validateServer(serverToken_t & server, ServerConfig & s)
{
    for(int i = 0; i < server.locations.size(); i++)
        s.AddLocation(Location());

    validateNumiricValues(server, s);
    validateboolValues(server, s);
    validateStringValues(server, s);
    validateErrorPages(server, s);
    validateAllowMethods(server, s);
    validateCgiPass(server, s);
    validateRedirections(server, s);

}