#ifndef CONFIGFILETYPES_HPP
#define CONFIGFILETYPES_HPP

#include <string>

typedef struct serverElementsCounter_s
{
    unsigned char    listen;
    unsigned char    sever_name;
    unsigned char    root;
    unsigned char    index;
    unsigned char    client_body_size;
    unsigned char    cgi_pass;
    unsigned char    error_pages;
} serverElementsCounter_t;

typedef struct locationElementsCounter_s
{
    unsigned char path;
    unsigned char index;
    unsigned char root;
    unsigned char upload_path;
    unsigned char auto_index;
    unsigned char redirection;
    unsigned char allow_methods;
    unsigned char client_body_size;
} locationElementsCounter_t;

typedef struct errorPages_s
{
    std::string errorCode;
    std::string pagePath;
} errorPages_t ;


typedef struct cgiPass_s {
    std::string extension;
    std::string path;
} cgiPass_t;

typedef struct redirection_s {
    std::string new_path;
    std::string status;
} redirection_t;

typedef struct location_s
{
    std::string path;
    std::string index;
    std::string root;
    std::string upload_path;
    std::string auto_index;
    redirection_t redirection;
    std::vector <std::string> allow_methods;
    std::string client_body_size;
    locationElementsCounter_t locationElementsCounter;
} location_t;

typedef struct serverToken_t {
    std::string listen;
    std::string sever_name;
    std::string root;
    std::string index;
    std::string client_body_size;
    std::vector < cgiPass_t > cgi_pass;
    std::vector < errorPages_t > error_pages;
    std::vector < location_t > locations;
    serverElementsCounter_t serevrElementsCounter;
} serverToken_t;



typedef std::vector<std::string> Configtokens;

#endif