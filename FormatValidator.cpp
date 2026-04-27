#include "configFile.hpp"



//check if we will gonna save another white space in the newConf 
bool isDoubleSpace(std::string &conf, std::string &newConf, int idx)
{
    return (std::isspace(conf[idx]) && (std::isspace(*(newConf.end() - 1)) || newConf.empty()));
}

//makes newConf without double white spaces
std::string removeDoubleSpaces(std::string &conf)
{
    std::string newConf;
    size_t i = 0;

    while (i < conf.length())
    {
        if (isDoubleSpace(conf, newConf, i))
        {
            i++;
            continue;
        }
        else
        {
            newConf += conf[i];
            i++;
        }
    }
    if (std::isspace(*(newConf.end() - 1)))
        newConf.pop_back();
    return newConf;
}

//checks if the character is a config file special character
//special characters are the onces that contain content blocks
bool isSpecialCharacter(int c)
{
    return c == '{' || c == '}' || c == ';';
}

//makes sure every special character have one space in it's sides
std::string fixSpecialCharactersPadding(std::string &conf)
{
    std::string newConf;
    for (size_t i = 0; i < conf.length(); i++)
    {
        if (isSpecialCharacter(conf[i]))
        {
            if (i != 0 && !std::isspace(*(newConf.end() - 1)))
			{
                newConf += ' ';
			}

            newConf += conf[i];

            if (conf[i + 1] && !std::isspace(conf[i + 1]))
			{
                newConf += ' ';
			}
        }
        else
            newConf += conf[i];
    }
    return newConf;
}

//convert white spaces to space '32'
void turnAllWhiteSpacesToSpace(std::string &conf)
{
    for(size_t i = 0; i != conf.length(); i++)
    {
        if (std::isspace(conf[i]))
            conf[i] = ' ';
    }
}

void normalizeConfigFormat(std::string &conf)
{

    std::string newConf = removeDoubleSpaces(conf);
    newConf = fixSpecialCharactersPadding(newConf);
    turnAllWhiteSpacesToSpace(newConf);

    conf = newConf;
}

