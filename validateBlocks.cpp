#include "configFile.hpp"

bool validateBlocks( std::string & conf)
{
    int p = 0;
    for(size_t i = 0; i < conf.length() ; ++i)
    {
        switch (conf[i])
        {
            case ('{'): p++; break;
            case ('}'): p--; break;
        }
        if (p < 0) return false;
    }
    return (p == 0);
}