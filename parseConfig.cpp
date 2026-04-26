#include "include/configFile.hpp"


void detectServerStartEnd(Configtokens & t, std::vector <std::pair < int, int> > & servers)
{
    std::pair<int, int> s;
    bool server = false;
    int p = 0;

    for (size_t i = 0; i < t.size(); i++)
    {
        if (!server)
        {
            if (t[i] == "server")
            {
                server = true ;
                s.first = i;
            }
            else
               throw ConfigException(": Config file must start with a 'server'") ;
        }
        else
        {
            if (t[i] == "{") p++;
            else if(t[i] == "}") p--;
            if (p == 0)
            {
            std::cout << "\n";
                s.first += 2;
                s.second = i - 1;
                server = false;
                servers.push_back(s);
            }
        }
    }
}

bool isServerKey(std::string & str)
{
    return (
        str == "listen" ||
        str == "server_name" ||
        str == "root" ||
        str == "index" ||
        str == "client_body_size"
    );
}
bool isServerblock(std::string & str)
{
    return (
        str == "error_page" ||
        str == "location" || 
        str == "cgi_pass"
    );
}

void extractSingleValue(Configtokens &t, int & idx, std::string target, std::string & dst)
{
    idx++;
    dst = t[idx];
    idx++;
    if (t[idx] != ";")
        throw  ConfigException(target + " have multiple Values");
}

bool isLocation(Configtokens & t, int idx)
{
    if (t[idx] == "location")
    {
        idx++;
        if(isWord(t[idx]))
        {
            idx++;
            if (t[idx] == "{")
                return true;
            else 
                return false;
        }
        else
            return false;
    }  
    else
        return false;

}

void extractReturnValue(Configtokens & t, int & idx, redirection_t & dst)
{
    idx++;
    if (isWord(t[idx]))
    {
        dst.status = t[idx];
        idx++;
        if (isWord(t[idx]))
        {
            dst.new_path = t[idx];
            idx++;
            if (t[idx] == ";")
                return ;
            else
                throw ConfigException("redirection have unknown values >" + t[idx]);
        }
        else
            throw ConfigException("redirection new_path not Found >" + t[idx]);
    }
    else
        throw ConfigException("redirection status not Found >" + t[idx]);
}

void extractMethodValues(Configtokens & t, int & idx, std::vector <std::string> & dst)
{
    idx++;
    while (t[idx] != ";")
    {
        dst.push_back(std::string(t[idx]));
        idx++;
    }
}

void extractCgiValues(Configtokens & t, int & idx, std::vector <cgiPass_t> & dst)
{
    idx++;
    if (t[idx] == "{")
    {
        idx++;
        while (t[idx] != "}")
        {
            cgiPass_t c;
            c.extension = t[idx];
            idx++;
            c.path = t[idx];
            idx++;
            if (t[idx] != ";")
            {
                throw ConfigException("unknown cgi_pass value "+ t[idx]);
            }
            else
                dst.push_back(c);
            idx++;
        }
        
    }
    else
    {
        throw ConfigException("invalid cgi_pass block");
    }

}

void extractErrorPageValues(Configtokens & t, int & idx, std::vector < errorPages_t> & dst)
{
    idx++;
    if (t[idx] == "{")
    {
        idx++;
        while (t[idx] != "}")
        {
            errorPages_t e;
            e.errorCode = t[idx]; // status code
            idx++;
            e.pagePath = t[idx];// html page
            idx++;
            if (t[idx] != ";")
                throw ConfigException("unknown at error_page value >"+t[idx]);
            else
                dst.push_back(e);
            idx++;
        }
        
    }
    else
        throw ConfigException("invalid at error_page block");
}
void printToken(const serverToken_t& t);

void ConvertConfigtikensToServerTokens(Configtokens & t, std::pair<int, int> borders, serverToken_t & server)
{
    for(int i = borders.first; i <= borders.second; i++)
    {
        if (isServerKey(t[i]))
        {
            
            if (t[i] == "listen")
            {
                extractSingleValue(t, i, "listen", server.listen);
                server.serevrElementsCounter.listen++;
            }
            else if (t[i] == "server_name")
            {
                extractSingleValue(t, i, "server_name", server.sever_name);
                server.serevrElementsCounter.sever_name++;
            }
            else if (t[i] == "root")
            {
                extractSingleValue(t, i, "root", server.root);
                server.serevrElementsCounter.root++;
            }
            else if (t[i] == "index")
            {
                extractSingleValue(t, i, "index", server.index);
                server.index = "/" + server.index;
                server.serevrElementsCounter.index++;
            }
            else if (t[i] == "client_body_size")
            {
                extractSingleValue(t, i, "client_body_size", server.client_body_size);
                server.serevrElementsCounter.client_body_size++;
            }
            else 
                throw ConfigException("unknown Key :" + t[i]);

        }
        else if (isServerblock(t[i]))
        {
            if (isLocation(t, i))
            {

                location_t l ;
                l.locationElementsCounter = (locationElementsCounter_t){0};
                l.path = t[++i];
                l.locationElementsCounter.path++;
                i += 2;
                int p = 1;
                while (t[i] != "}")
                {
                    if (t[i] == "root")
                    {
                        extractSingleValue(t, i, "root", l.root);
                        l.locationElementsCounter.root++;
                    }
                    else if (t[i] == "index")
                    {
                        extractSingleValue(t, i, "index", l.index);
                        l.index = "/" + l.index;
                        l.locationElementsCounter.index++;
                    }
                    else if (t[i] == "client_body_size")
                    {
                        extractSingleValue(t, i, "client_body_size", l.client_body_size);
                        l.locationElementsCounter.client_body_size++;
                    }
                    else if (t[i] == "auto_index")
                    {
                        extractSingleValue(t, i, "auto_index", l.auto_index);
                        l.locationElementsCounter.auto_index++;
                    }
                    else if (t[i] == "upload_path")
                    {
                        extractSingleValue(t, i, "upload_path", l.upload_path);
                        l.locationElementsCounter.upload_path++;
                    }
                    else if (t[i] == "return")
                    {
                        extractReturnValue(t, i, l.redirection);
                        l.locationElementsCounter.redirection++;
                    }
                    else if (t[i] == "allow_methods")
                    {
                        extractMethodValues(t, i, l.allow_methods);
                        l.locationElementsCounter.allow_methods++;
                    }
                    else 
                        throw ConfigException("unknown Key :" + t[i]);

                    i++;
                }
                server.locations.push_back(l);
            }

            else if (t[i] == "error_page")
            {
                extractErrorPageValues(t, i, server.error_pages);
                server.serevrElementsCounter.error_pages++;
            }

            else if (t[i] == "cgi_pass")
            {
                extractCgiValues(t, i, server.cgi_pass);
                server.serevrElementsCounter.cgi_pass++;
            }

        }

        else
            throw ConfigException("unixpected Key :" + t[i]);
        
    }
    // printToken(server); //debugging

}

void printToken(const serverToken_t & t)
{
    std::cout << BOLD << BLUE << "\n===== SERVER =====\n" << RESET;

    std::cout << CYAN << "listen: " << RESET << t.listen << "\n";
    std::cout << CYAN << "server_name: " << RESET << t.sever_name << "\n";
    std::cout << CYAN << "root: " << RESET << t.root << "\n";
    std::cout << CYAN << "index: " << RESET << t.index << "\n";
    std::cout << CYAN << "client_body_size: " << RESET << t.client_body_size << "\n";

    // 🔴 Error Pages
    std::cout << BOLD << RED << "\n-- Error Pages --\n" << RESET;
    for (size_t i = 0; i < t.error_pages.size(); i++)
    {
        std::cout << YELLOW << "[" << i << "] "
                  << "code: " << RESET << t.error_pages[i].errorCode
                  << YELLOW << " path: " << RESET << t.error_pages[i].pagePath
                  << "\n";
    }
    // ⚙️ CGI

    std::cout << GREEN << "cgi_pass:\n" << RESET;
    for (size_t j = 0; j < t.cgi_pass.size(); j++)
    {
        std::cout << "  " << CYAN << t.cgi_pass[j].extension
                    << RESET << " -> "
                    << t.cgi_pass[j].path << "\n";
    }

    // 🟢 Locations
    std::cout << BOLD << GREEN << "\n-- Locations --\n" << RESET;

    for (size_t i = 0; i < t.locations.size(); i++)
    {
        const location_t loc = t.locations[i];

        std::cout << BOLD << "\n[Location " << i << "]\n" << RESET;

        std::cout << CYAN << "path: " << RESET << loc.path << "\n";
        std::cout << CYAN << "root: " << RESET << loc.root << "\n";
        std::cout << CYAN << "index: " << RESET << loc.index << "\n";
        std::cout << CYAN << "upload_path: " << RESET << loc.upload_path << "\n";
        std::cout << CYAN << "auto_index: " << RESET << loc.auto_index << "\n";
        std::cout << CYAN << "client_body_size: " << RESET << loc.client_body_size << "\n";

        // Methods
        std::cout << YELLOW << "allowed methods: " << RESET;
        for (size_t j = 0; j < loc.allow_methods.size(); j++)
            std::cout << loc.allow_methods[j] << " ";
        std::cout << "\n";



        // 🔁 Redirection
        if (!loc.redirection.new_path.empty())
        {
            std::cout << RED << "redirection -> " << RESET
                      << loc.redirection.status << " "
                      << loc.redirection.new_path << "\n";
        }

        
    }

    std::cout << BLUE << "=====================\n" << RESET;
}

void detectDuplication(serverToken_t & s)
{
    for (int idx = 0; idx < sizeof(serverElementsCounter_t); idx++)
    {
        unsigned char *val = (unsigned char *)&s.serevrElementsCounter + idx;
        if (*val > 1) 
            throw ConfigException("duplication in server element has been detected ");
    }
    for (int i = 0; i < s.locations.size(); i++)
    {
        for (int idx = 0; idx < sizeof(locationElementsCounter_t); idx++)
        {
            unsigned char *val = (unsigned char *)&s.locations[i].locationElementsCounter + idx;
            if (*val > 1) 
                throw ConfigException("duplication in location element has been detected");
        }
    }
}
void parseConfig(Configtokens & tokens, std::vector<ServerConfig> & servers)
{
    std::vector < std::pair < int,int > > serverStartEnd;

    detectServerStartEnd(tokens, serverStartEnd);

    for (int i = 0; i < serverStartEnd.size(); i++)
    {
        serverToken_t serverToken;
        serverToken.serevrElementsCounter = (serverElementsCounter_t){0};
        ConvertConfigtikensToServerTokens(tokens, serverStartEnd[i], serverToken);
        detectDuplication(serverToken);


        ServerConfig newServer;
        validateServer(serverToken, newServer);
        newServer.PrintServerInfo();
        servers.push_back(newServer);
    }
}